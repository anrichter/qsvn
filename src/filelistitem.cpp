/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
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
#include "filelistitem.h"
#include "statustext.h"

//Qt
#include <QtGui>

//Svn
#include "svncpp/client.hpp"
#include "svncpp/path.hpp"
#include "svncpp/revision.hpp"
#include "svncpp/wc.hpp"
#include <svn_wc.h>


FileListItem::FileListItem( const QList< QVariant > &data, FileListItem *parent )
{
    parentItem = parent;
    itemData = data;
    sortColumn = FileListItem::FilenameColumn;
}

FileListItem::FileListItem( const FileListItem *fileListItem, FileListItem *parent )
{
    parentItem = parent;
    for ( int i = 0; i < fileListItem->columnCount(); ++i )
    {
        itemData << fileListItem->data( i );
        sortColumn = FileListItem::FilenameColumn;
    }
}

FileListItem::~FileListItem()
{
    qDeleteAll( childItems );
}

void FileListItem::appendChild( FileListItem *child )
{
    childItems.append( child );
}

void FileListItem::removeChild( int row )
{
    childItems.removeAt( row );
}

FileListItem * FileListItem::child( int row )
{
    return childItems.value( row );
}

int FileListItem::childCount( ) const
{
    return childItems.count();
}

int FileListItem::columnCount( ) const
{
    return itemData.count();
}

QVariant FileListItem::data( int column ) const
{
    return itemData.value( column );
}

void FileListItem::setData( int column, QVariant value )
{
    itemData.insert( column, value );
}

int FileListItem::row( ) const
{
    if ( parentItem )
        return parentItem->childItems.indexOf( const_cast<FileListItem*>( this ) );

    return 0;
}

FileListItem * FileListItem::parent( )
{
    return parentItem;
}

int FileListItem::getSortColumn()
{
    return sortColumn;
}

void FileListItem::setSortColumn( int column )
{
    sortColumn = column;
}

bool FileListItem::operator < (const FileListItem &other) const
{
    if ( sortColumn == FileListItem::RevisionColumn ) //revision
        return data( FileListItem::RevisionColumn ).toInt() < other.data( FileListItem::RevisionColumn ).toInt();
    else
        return data( sortColumn ).toString() < other.data( sortColumn ).toString();
    return true;
}

QPixmap FileListItem::getPixmap( )
{
    int statusKind = itemData.value( FileListItem::StatusColumn ).toInt();
    switch ( statusKind )
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

QString FileListItem::fullFileName( )
{
    QFileInfo fileInfo = QFileInfo( itemData.value( FileListItem::FullfilenameColumn ).toString() );
    return fileInfo.canonicalFilePath();
}
