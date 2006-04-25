/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2005 Andreas Richter <ar@oszine.de>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License Version 2        *
 *   as published by the Free Software Foundation.                         *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 ***************************************************************************/

//QSvn
#include "config.h"
#include "logentriesmodel.h"
#include "logchangepathentriesmodel.h"
#include "showlog.h"
#include "svnclient.h"

//svnqt
#include "svnqt/client.hpp"
#include "svnqt/log_entry.hpp"
#include "svnqt/wc.hpp"

//Qt
#include <QtGui>


ShowLog::ShowLog( QWidget *parent, const QString path, const svn::LogEntries *logEntries )
    : QDialog( parent )
{
    setupUi( this );
    Config::instance()->restoreWidget( this );
    Config::instance()->restoreSplitter( this, splitter );

    m_logEntriesModel = new LogEntriesModel( this, logEntries );
    viewLogEntries->setModel( m_logEntriesModel );
    Config::instance()->restoreHeaderView( this, viewLogEntries->header() );

    connect( viewLogEntries, SIGNAL( clicked( const QModelIndex & ) ), this, SLOT( selectLogEntry( const QModelIndex & ) ) );

    m_logChangePathEntriesModel = new LogChangePathEntriesModel( this, svn::LogChangePathEntries() );
    viewLogChangePathEntries->setModel( m_logChangePathEntriesModel );
    viewLogChangePathEntries->installEventFilter( this );
    Config::instance()->restoreHeaderView( this, viewLogChangePathEntries->header() );
    connect( viewLogChangePathEntries, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( doDiff() ) );
    
    contextLogChangePathEntries = new QMenu( this );
    contextLogChangePathEntries->addAction( actionDiff );
    connectActions();

    m_path = path;
    m_path.replace( "\\", "/" );
    if ( !m_path.endsWith( "/" ) )
        m_path.append( "/" );
}

ShowLog::~ShowLog()
{
    delete m_logEntriesModel;
    delete contextLogChangePathEntries;
    Config::instance()->saveWidget( this );
    Config::instance()->saveSplitter( this, splitter );
    Config::instance()->saveHeaderView( this, viewLogEntries->header() );
    Config::instance()->saveHeaderView( this, viewLogChangePathEntries->header() );
}

void ShowLog::doShowLog( QWidget *parent, const QString path, const svn::Revision revisionStart, const svn::Revision revisionEnd )
{
    const svn::LogEntries *logEntries;
    logEntries = SvnClient::instance()->log( path, revisionStart, revisionEnd );

    ShowLog *showLog;
    showLog = new ShowLog( parent, path, logEntries );
    showLog->show();
    showLog->raise();
    showLog->activateWindow();
}

bool ShowLog::eventFilter( QObject * watched, QEvent * event )
{
    if ( watched == viewLogChangePathEntries )
    {
        if ( event->type() == QEvent::ContextMenu )
        {
            contextLogChangePathEntries->popup( static_cast< QContextMenuEvent* >( event )->globalPos() );
        }
    }
    return QDialog::eventFilter( watched, event );
}

void ShowLog::selectLogEntry( const QModelIndex & index )
{
    if ( index.isValid() )
    {
        Config::instance()->saveHeaderView( this, viewLogChangePathEntries->header() );
        editLogMessage->setPlainText( m_logEntriesModel->getLogEntry( index ).message );
        delete m_logChangePathEntriesModel;
        m_logChangePathEntriesModel = new LogChangePathEntriesModel( this, m_logEntriesModel->getLogEntry( index ).changedPaths );
        viewLogChangePathEntries->setModel( m_logChangePathEntriesModel );
        Config::instance()->restoreHeaderView( this, viewLogChangePathEntries->header() );
    }
}

void ShowLog::connectActions( )
{
    connect( actionDiff, SIGNAL( triggered() ), this, SLOT( doDiff() ) );
}

void ShowLog::doDiff( )
{
    svn::LogEntry logEntry;
    svn::LogChangePathEntry logChangePathEntry;

    QModelIndexList indexes;
    indexes = viewLogEntries->selectionModel()->selectedIndexes();
    if (indexes.count() == 0 )
        return;
    logEntry = m_logEntriesModel->getLogEntry( indexes.at( 0 ) );

    indexes = viewLogChangePathEntries->selectionModel()->selectedIndexes();
    if ( indexes.count() == 0 )
        return;
    logChangePathEntry = m_logChangePathEntriesModel->getLogChangePathEntry( indexes.at( 0 ) );

    SvnClient::instance()->diff( QString( svn::Wc::getRepos( m_path ) + logChangePathEntry.path ), 
                                 svn::Revision( logEntry.revision - 1 ), svn::Revision( logEntry.revision ) );
}
