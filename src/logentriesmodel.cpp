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
#include "logentriesmodel.h"

//SvnQt
#include "svnqt/client.hpp"

//Qt
#include <QtCore>


LogEntriesModel::LogEntriesModel( QObject * parent, const svn::LogEntries *logEntries )
    : QAbstractTableModel( parent )
{
    m_logEntries = svn::LogEntries( *logEntries );
    sort( 0, Qt::DescendingOrder );
}

LogEntriesModel::~LogEntriesModel()
{}

int LogEntriesModel::rowCount( const QModelIndex & parent ) const
{
    return m_logEntries.count();
}

int LogEntriesModel::columnCount( const QModelIndex & parent ) const
{
    return 4;
}

QVariant LogEntriesModel::headerData( int section, Qt::Orientation orientation, int role ) const
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

QVariant LogEntriesModel::data( const QModelIndex & index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();
    if ( role != Qt::DisplayRole )
        return QVariant();

    const svn::LogEntry &logEntry = m_logEntries.at( index.row() );
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
        return logEntry.message.simplified();
        break;
    }
}

void LogEntriesModel::sort( int column, Qt::SortOrder order )
{
    if ( column != 0 )
        return;

    if ( order == Qt::AscendingOrder )
        qSort( m_logEntries.begin(), m_logEntries.end(), &logEntryLessThan );
    else
        qSort( m_logEntries.begin(), m_logEntries.end(), &logEntryGreaterThan );

    emit layoutChanged();
}

bool LogEntriesModel::logEntryLessThan( const svn::LogEntry & left, const svn::LogEntry & right )
{
    return left.revision < right.revision;
}

bool LogEntriesModel::logEntryGreaterThan( const svn::LogEntry & left, const svn::LogEntry & right )
{
    return left.revision > right.revision;
}

svn::LogEntry LogEntriesModel::getLogEntry( const QModelIndex & index )
{
    return m_logEntries.at( index.row() );
}
