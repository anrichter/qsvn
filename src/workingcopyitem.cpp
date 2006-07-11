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
#include "workingcopyitem.h"
#include "statustext.h"

//SvnCpp
#include "svnqt/client.hpp"
#include "svnqt/path.hpp"
#include "svnqt/revision.hpp"
#include "svnqt/wc.hpp"

//Qt
#include <QtGui>


WorkingCopyItem::WorkingCopyItem( const QList< QVariant > &data, WorkingCopyItem *parent )
{
    parentItem = parent;
    itemData = data;

    svnDirectory = svn::Wc::checkWc( itemData.value( 1 ).toString().toLocal8Bit() );
}

WorkingCopyItem::~WorkingCopyItem()
{
    deleteAllChilds();
}

void WorkingCopyItem::appendChild( WorkingCopyItem *child )
{
    childItems.append( child );
}

void WorkingCopyItem::removeChild( int row )
{
    childItems.removeAt( row );
}

void WorkingCopyItem::deleteAllChilds()
{
    qDeleteAll( childItems );
}

WorkingCopyItem * WorkingCopyItem::child( int row )
{
    return childItems.value( row );
}

int WorkingCopyItem::childCount( ) const
{
    return childItems.count();
}

int WorkingCopyItem::columnCount( ) const
{
    return itemData.count();
}

QVariant WorkingCopyItem::data( int column ) const
{
    return itemData.value( column );
}

int WorkingCopyItem::row( ) const
{
    if ( parentItem )
        return parentItem->childItems.indexOf( const_cast<WorkingCopyItem*>( this ) );

    return 0;
}

WorkingCopyItem * WorkingCopyItem::parent( )
{
    return parentItem;
}

QPixmap WorkingCopyItem::getPixmap()
{
    if ( svnDirectory )
        return QPixmap( ":folder.png" );
    else
        return QPixmap( ":unknownfolder.png" );
}

QString WorkingCopyItem::fullPath() const
{
    QFileInfo fileInfo = QFileInfo( itemData.value( 1 ).toString() );
    return fileInfo.canonicalFilePath();
}

QStringList WorkingCopyItem::fullPathListRecursive( QStringList fullPathList )
{
    fullPathList << fullPath();

    for ( int i = 0; i < childCount(); i++ )
    {
        fullPathList = child( i )->fullPathListRecursive( fullPathList );
    }
    return fullPathList;
}

