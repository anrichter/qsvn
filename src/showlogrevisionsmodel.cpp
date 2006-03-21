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
#include "showlogrevisionsmodel.h"

//SvnQt
#include "svnqt/client.hpp"

//Qt
#include <QtCore>


ShowLogRevisionsModel::ShowLogRevisionsModel( QObject * parent, const svn::LogEntries *logEntries )
    : QAbstractTableModel( parent )
{
    m_logEntries = logEntries;
}

ShowLogRevisionsModel::~ShowLogRevisionsModel()
{}

int ShowLogRevisionsModel::rowCount( const QModelIndex & parent ) const
{
    return m_logEntries->count();
}

int ShowLogRevisionsModel::columnCount( const QModelIndex & parent ) const
{
    return 4;
}

QVariant ShowLogRevisionsModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch ( section )
        {
        case 0:
            return QString( tr( "Revision" ) );
            break;
        case 1:
            return QString( tr( "Author" ) );
            break;
        case 2:
            return QString( tr( "Date" ) );
            break;
        case 3:
            return QString( tr( "Message" ) );
            break;
        }
    }
    return QVariant();
}

QVariant ShowLogRevisionsModel::data( const QModelIndex & index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();
    if ( role != Qt::DisplayRole )
        return QVariant();

    const svn::LogEntry &logEntry = m_logEntries->at( index.row() );
    QDateTime dateTime;
    switch( index.column() )
    {
    case 0:
        return int( logEntry.revision );
        break;
    case 1:
        return logEntry.author;
        break;
    case 2:
        dateTime.setTime_t( logEntry.date / (1000*1000) );
        return dateTime.toString( "yyyy-MM-dd hh:mm:ss" );
        break;
    case 3:
        return logEntry.message;
        break;
    }
}
