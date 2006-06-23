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

//Qt
#include <QtGui>


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
                    return statusString( status );
                    break;
                case 2:
                    if ( status.isVersioned() )
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
        case Qt::DecorationRole:
            if ( index.column() == 0 )
                return statusPixmap( status );
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

QPixmap StatusEntriesModel::statusPixmap( svn::Status status ) const
{
    switch ( status.textStatus() )
    {
    case svn_wc_status_none:
        return QPixmap( ":file.png" );
    case svn_wc_status_unversioned:
        return QPixmap( ":unknownfile.png" );
    case svn_wc_status_normal:
        return QPixmap( ":file.png" );
    case svn_wc_status_added:
        return QPixmap( ":addedfile.png" );
    case svn_wc_status_missing:
        return QPixmap( ":missingfile.png" );
    case svn_wc_status_deleted:
        return QPixmap( ":modifiedfile.png" );
    case svn_wc_status_replaced:
        return QPixmap( ":modifiedfile.png" );
    case svn_wc_status_modified:
        return QPixmap( ":modifiedfile.png" );
    case svn_wc_status_merged:
        return QPixmap( ":modifiedfile.png" );
    case svn_wc_status_conflicted:
        return QPixmap( ":conflictedfile.png" );
    case svn_wc_status_ignored:
        return QPixmap( ":file.png" );
    case svn_wc_status_obstructed:
        return QPixmap( ":file.png" );
    case svn_wc_status_external:
        return QPixmap( ":file.png" );
    case svn_wc_status_incomplete:
        return QPixmap( ":file.png" );
    default:
        return QPixmap( ":file.png" );
    }
}

QString StatusEntriesModel::statusString( svn::Status status ) const
{
        switch ( status.textStatus() )
        {
        case svn_wc_status_none:
            return QString( tr( "none" ) );
        case svn_wc_status_unversioned:
            return QString( tr( "unversioned" ) );
        case svn_wc_status_normal:
            return QString( tr( "normal" ) );
        case svn_wc_status_added:
            return QString( tr( "added" ) );
        case svn_wc_status_missing:
            return QString( tr( "missing" ) );
        case svn_wc_status_deleted:
            return QString( tr( "deleted" ) );
        case svn_wc_status_replaced:
            return QString( tr( "replaced" ) );
        case svn_wc_status_modified:
            return QString( tr( "modified" ) );
        case svn_wc_status_merged:
            return QString( tr( "merged" ) );
        case svn_wc_status_conflicted:
            return QString( tr( "conflicted" ) );
        case svn_wc_status_ignored:
            return QString( tr( "ignored" ) );
        case svn_wc_status_obstructed:
            return QString( tr( "obstructed" ) );
        case svn_wc_status_external:
            return QString( tr( "external" ) );
        case svn_wc_status_incomplete:
            return QString( tr( "incomplete" ) );
        default:
            return QString( status.textStatus() );
        }
}
