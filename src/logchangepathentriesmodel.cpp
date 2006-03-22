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
#include "logchangepathentriesmodel.h"

//SvnQt
#include "svnqt/client.hpp"
#include "svnqt/log_entry.hpp"

//Qt
#include <QtCore>


LogChangePathEntriesModel::LogChangePathEntriesModel( QObject * parent, svn::LogChangePathEntries logChangePathEntries )
        : QAbstractTableModel( parent )
{
    m_logChangePathEntries = logChangePathEntries;
}

LogChangePathEntriesModel::~LogChangePathEntriesModel()
{}

int LogChangePathEntriesModel::rowCount( const QModelIndex & parent ) const
{
    return m_logChangePathEntries.count();
}

int LogChangePathEntriesModel::columnCount( const QModelIndex & parent ) const
{
    return 2;
}

QVariant LogChangePathEntriesModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch ( section )
        {
        case 0:
            return QString( tr( "Path" ) );
            break;
        case 1:
            return QString( tr( "Action" ) );
            break;
            //      QString copyFromPath;
            //      svn_revnum_t copyFromRevision;
            //      QString copyToPath;
            //      svn_revnum_t copyToRevision;
        }
    }
    return QVariant();
}

QVariant LogChangePathEntriesModel::data( const QModelIndex & index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();
    if ( role != Qt::DisplayRole )
        return QVariant();

    svn::LogChangePathEntry logChangePathEntry = m_logChangePathEntries.at( index.row() );

    switch( index.column() )
    {
    case 0:
        return logChangePathEntry.path;
        break;
    default:
        return QVariant();
        break;
    }
}
