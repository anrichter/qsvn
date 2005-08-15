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
#include <QVariant>


FileListItem::FileListItem( const QList< QVariant > &data, FileListItem *parent )
{
    parentItem = parent;
    itemData = data;
}

FileListItem::~FileListItem()
{
    deleteAllChilds();
}

void FileListItem::deleteAllChilds()
{
    qDeleteAll( childItems );
}

void FileListItem::appendChild( FileListItem *child )
{
    childItems.append( child );
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
