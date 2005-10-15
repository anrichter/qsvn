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
 *   As a special exception, permission is given to link this program      *
 *   with any edition of Qt, and distribute the resulting executable,      *
 *   without including the source code for Qt in the source distribution.  *
 ***************************************************************************/


//QSvn
#include "filelistitem.h"

//Qt
#include <QList>
#include <QPixmap>
#include <QVariant>

//Svn
#include <svn_wc.h>


FileListItem::FileListItem( const QList< QVariant > &data, FileListItem *parent )
{
    parentItem = parent;
    itemData = data;
    sortColumn = 0;
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
    if ( column == 1 )
    {
        int statusKind = itemData.value( 1 ).toInt();
        switch ( statusKind )
        {
        case svn_wc_status_none:
            return QString( "none" );
        case svn_wc_status_unversioned:
            return QString( "unversioned" );
        case svn_wc_status_normal:
            return QString( "normal" );
        case svn_wc_status_added:
            return QString( "added" );
        case svn_wc_status_missing:
            return QString( "missing" );
        case svn_wc_status_deleted:
            return QString( "deleted" );
        case svn_wc_status_replaced:
            return QString( "replaced" );
        case svn_wc_status_modified:
            return QString( "modified" );
        case svn_wc_status_merged:
            return QString( "merged" );
        case svn_wc_status_conflicted:
            return QString( "conflicted" );
        case svn_wc_status_ignored:
            return QString( "ignored" );
        case svn_wc_status_obstructed:
            return QString( "obstructed" );
        case svn_wc_status_external:
            return QString( "external" );
        case svn_wc_status_incomplete:
            return QString( "incomplete" );
        default:
            return itemData.value( 1 ).toString();
        }
    }
    else
    {
        return itemData.value( column );
    }
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
    if ( sortColumn == 2 ) //revision
        return data( 2 ).toInt() < other.data( 2 ).toInt();
    else
        return data( sortColumn ).toString() < other.data( sortColumn ).toString();
    return true;
}

QPixmap FileListItem::getPixmap( )
{
    int statusKind = itemData.value( 1 ).toInt();
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
