/***************************************************************************
 *   This file is part of QSvn Project http://ar.oszine.de/projects/qsvn   *
 *   Copyright (c) 2004-2006 Andreas Richter <ar@oszine.de>                *
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
#include "fileselector.h"
#include "fileselectorproxy.h"
#include "svnclient.h"

//Qt
#include <QtGui>


FileSelector::FileSelector( QWidget * parent, SvnClient::SvnAction svnAction, QString wc )
        : QDialog( parent )
{
    setupUi( this );
    //initModelProxy
    m_fileSelectorProxy = new FileSelectorProxy( parent, svnAction );
    m_fileSelectorProxy->readDirectory( wc, true );
    setupFileSelector( svnAction );
}

FileSelector::FileSelector( QWidget *parent, SvnClient::SvnAction svnAction,  
                            QStringList fileList )
        : QDialog( parent )
{
    setupUi( this );
    //initModelProxy
    m_fileSelectorProxy = new FileSelectorProxy( parent, svnAction );
    m_fileSelectorProxy->readFileList( fileList );
    setupFileSelector( svnAction );
}

FileSelector::~FileSelector( )
{
    Config::instance()->saveWidget( this, this->windowTitle() );
    Config::instance()->setValue( "selectAll" + this->windowTitle(), checkSelectAll->checkState() );
    if ( m_fileSelectorProxy )
        delete( m_fileSelectorProxy );
}

void FileSelector::setupFileSelector( SvnClient::SvnAction svnAction )
{
    m_svnAction = svnAction;
    
    treeViewFiles->setModel( m_fileSelectorProxy );

    setupMenus();
    setupUI();
    setupConnections();
    
    treeViewFiles->installEventFilter( this );

    Config::instance()->restoreWidget( this, this->windowTitle() );
    Config::instance()->restoreHeaderView( this, treeViewFiles->header() );
}

void FileSelector::setupUI()
{
    switch ( m_svnAction )
    {
    case SvnClient::SvnNone:
        setWindowTitle( "" );
        hideGroupBoxLogMessage();
        break;
    case SvnClient::SvnAdd:
        setWindowTitle( tr( "Add") );
        setWindowIcon( QIcon( ":actionaddlocal.png" ) );
        hideGroupBoxLogMessage();
        break;
    case SvnClient::SvnCommit:
        setWindowTitle( tr( "Commit") );
        setWindowIcon( QIcon( ":actioncommit.png" ) );
        comboLogHistory->addItems( Config::instance()->getStringList( "logHistory" ) );
        comboLogHistory->insertItem( 0, "" );
        comboLogHistory->setCurrentIndex( 0 );
        Config::instance()->restoreSplitter( this, splitter );
        break;
    case SvnClient::SvnDelete:
        setWindowTitle( tr( "Delete") );
        setWindowIcon( QIcon( ":actiondeletelocal.png" ) );
        hideGroupBoxLogMessage();
        break;
    case SvnClient::SvnRevert:
        setWindowTitle( tr( "Revert") );
        setWindowIcon( QIcon( ":actionrevert.png" ) );
        hideGroupBoxLogMessage();
        break;
    }
}

void FileSelector::setupMenus()
{
    contextMenu = new QMenu( this );

    if ( ( m_svnAction == SvnClient::SvnCommit ) ||
            ( m_svnAction == SvnClient::SvnRevert ) )
    {
        contextMenu->addAction( actionDiff );
        contextMenu->addAction( actionRevert );
        actionRevert->setIcon( QIcon( ":actionrevert.png" ) );
    }
}

void FileSelector::setupConnections( )
{
    connect( okButton, SIGNAL( clicked() ), this, SLOT( buttonOkClickedSlot() ) );
    connect( comboLogHistory, SIGNAL( activated( int ) ), this, SLOT( comboLogHistoryActivatedSlot( int ) ) );
    connect( checkSelectAll, SIGNAL( stateChanged( int ) ), this, SLOT( checkSelectAllStateChanged( int ) ) );
    connect( treeViewFiles->selectionModel(),
             SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
             this, SLOT( updateActions( const QItemSelection &, const QItemSelection & ) ) );

    connect( actionDiff, SIGNAL( triggered() ), this, SLOT( doDiff() ) );
    connect( actionRevert, SIGNAL( triggered() ), this, SLOT( doRevert() ) );
}

int FileSelector::exec()
{
    checkSelectAll->setCheckState( Qt::CheckState( Config::instance()->value( "selectAll" + this->windowTitle() ).toInt() ) );
    return QDialog::exec();
}

void FileSelector::hideGroupBoxLogMessage( )
{
    groupBoxLogMessage->setVisible( false );
}

QStringList FileSelector::checkedFileList()
{
    if ( m_fileSelectorProxy )
        return m_fileSelectorProxy->checkedFileList();
    else
        return QStringList();
}

QString FileSelector::logMessage( )
{
    return editLogMessage->toPlainText();
}

void FileSelector::buttonOkClickedSlot()
{
    if ( m_svnAction == SvnClient::SvnCommit )
    {
        QStringList logEntries;
        logEntries << editLogMessage->toPlainText();
        for ( int i = 0; i < comboLogHistory->count(); ++i )
        {
            if ( !comboLogHistory->itemText( i ).isEmpty() && !logEntries.contains( comboLogHistory->itemText( i ) ) )
                logEntries << comboLogHistory->itemText( i );
        }
        Config::instance()->saveStringList( "logHistory", logEntries );
        Config::instance()->saveSplitter( this, splitter );
    }

    Config::instance()->saveHeaderView( this, treeViewFiles->header() );
    this->accept();
}

void FileSelector::comboLogHistoryActivatedSlot( int index )
{
    editLogMessage->setPlainText( comboLogHistory->itemText( index ) );
}

void FileSelector::checkSelectAllStateChanged( int state )
{
    m_fileSelectorProxy->setSelectAllState( state );
}

bool FileSelector::eventFilter( QObject * watched, QEvent * event )
{
    if ( watched == treeViewFiles )
    {
        if ( event->type() == QEvent::ContextMenu )
            contextMenu->popup( static_cast< QContextMenuEvent* >( event )->globalPos() );
    }
    return QDialog::eventFilter( watched, event );
}

void FileSelector::doRevert( )
{
    QModelIndex index = treeViewFiles->selectionModel()->currentIndex();

    QString fullFileName;
    fullFileName = m_fileSelectorProxy->at( index ).path();

    if ( QMessageBox::question( this, tr( "Revert" ),
                                QString( tr( "Do you really want to revert local changes from\n%1?" ) ).arg( fullFileName ),
                                QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        SvnClient::instance()->revert( fullFileName, false );

        m_fileSelectorProxy->updateEntry( index );
    }
}

void FileSelector::diff( const QModelIndex &index )
{
    SvnClient::instance()->diff( m_fileSelectorProxy->at( index ).path() );
}

void FileSelector::doDiff( )
{
    diff( treeViewFiles->selectionModel()->currentIndex() );
}

void FileSelector::updateActions( const QItemSelection &selected, const QItemSelection &deselected )
{
    svn::Status _status = m_fileSelectorProxy->at( selected.indexes().at( 0 ) );

    if ( _status.textStatus() == svn_wc_status_modified )
    {
        actionDiff->setEnabled( true );
        connect( treeViewFiles, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( diff( const QModelIndex & ) ) );
    }
    else
    {
        actionDiff->setEnabled( false );
        disconnect( treeViewFiles, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( diff( const QModelIndex & ) ) );
    }
}

#include "fileselector.moc"
