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
#include "showlog.h"
#include "logentriesmodel.h"

//Qt
#include <QtGui>


ShowLog::ShowLog( QWidget *parent, const svn::LogEntries *logEntries )
    : QDialog( parent )
{
    setupUi( this );
    Config::instance()->restoreWidget( this );

    m_logEntriesModel = new LogEntriesModel( this, logEntries );
    viewRevisions->setModel( m_logEntriesModel );

    connect( viewRevisions, SIGNAL( clicked( const QModelIndex & ) ), this, SLOT( selectLogEntry( const QModelIndex & ) ) );
}

ShowLog::~ShowLog()
{
    delete m_logEntriesModel;
    Config::instance()->saveWidget( this );
}

void ShowLog::selectLogEntry( const QModelIndex & index )
{
    if ( index.isValid() )
    {
        editLogMessage->setPlainText( m_logEntriesModel->getLogEntry( index ).message );
    }
}
