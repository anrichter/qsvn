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
#include "statusentriesmodel.h"
#include "svnclient.h"

//SvnQt
#include "svnqt/client.hpp"


StatusEntriesModel::StatusEntriesModel( QObject * parent )
    : QAbstractTableModel( parent )
{
    m_statusEntries = svn::StatusEntries();
}

StatusEntriesModel::~StatusEntriesModel()
{
}

int StatusEntriesModel::rowCount( const QModelIndex &parent ) const
{
    return m_statusEntries.count();
}

int StatusEntriesModel::columnCount( const QModelIndex &parent ) const
{
    return 4;
}

QVariant StatusEntriesModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch ( section )
        {
            case 0:
                return QString( tr( "Filename" ) );
                break;
            case 1:
                return QString( tr( "Status" ) );
                break;
            case 2:
                return QString( tr( "Revision" ) );
                break;
            case 3:
                return QString( tr( "Author" ) );
                break;
            case 4:
                return QString( tr( "Path" ) );
                break;
        }
    }
    return QVariant();
}

QVariant StatusEntriesModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();
    if ( role != Qt::DisplayRole )
        return QVariant();

    svn::Status status = m_statusEntries.at( index.row() );
    QFileInfo fileInfo( status.path() );

    switch( role )
    {
        case Qt::DisplayRole:
            switch( index.column() )
            {
                case 0:
                    if ( !status.isVersioned() ||                         //return path for unversioned Files
                          ( status.isVersioned() && fileInfo.isDir() ) )  //            and for versioned Directories
                        return status.path().right( status.path().size() - m_directory.size() - 1  );
                    else
                        return status.entry().name();
                    break;
                case 1:
                    return status.textStatus();
                    break;
                case 2:
                    return int( status.entry().cmtRev() );
                    break;
                case 3:
                    return status.entry().cmtAuthor();
                    break;
                case 4:
                    return status.path();
                    break;
            }
            break;
    }
    return QVariant();
}

void StatusEntriesModel::readDirectory( QString directory )
{
    if ( m_directory != directory )
    {
        m_directory = directory;
        m_statusEntries = SvnClient::instance()->status( m_directory );
        emit layoutChanged();
    }
}

svn::Status StatusEntriesModel::at( int row )
{
    return m_statusEntries.at( row );
}
