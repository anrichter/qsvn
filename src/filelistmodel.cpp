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
#include "filelistmodel.h"

#include "statustext.h"

//SvnCpp
#include "svncpp/client.hpp"
#include "svncpp/status.hpp"

//Qt
#include <QtGui>


FileListModel::FileListModel( QObject *parent )
        : QAbstractItemModel( parent )
{
    QList< QVariant > rootData;
    rootData << "Filename" << "Status" << "Revision" << "Author";
    rootItem = new FileListItem( rootData );
    sortOrder = Qt::AscendingOrder;

    svnContext = 0;
}

FileListModel::~FileListModel()
{
    delete rootItem;
    if ( svnContext )
    {
        delete svnContext;
        svnContext = 0;
    }
}

void FileListModel::setActiveDirectory( QString directory )
{
    if ( directory.isEmpty() )
        return;
    if ( oldDirectory != directory )
    {
        removeRows( 0, rootItem->childCount() );
        QList< QVariant > columnData;

        //create a new svn::Context
        if ( svnContext )
        {
            delete svnContext;
            svnContext = 0;
        }

        svnContext = new svn::Context();
        oldDirectory = directory;

        svn::Client svnClient( svnContext );

        svn::StatusEntries statusList = svnClient.status( directory.toLocal8Bit(), false, true, false, false);
        svn::StatusEntries::iterator it;
        for ( it = statusList.begin(); it != statusList.end(); ++it )
        {
            if ( ! QDir( directory + QDir::separator() + QString( it->entry().name() ) ).exists() )
            {
                columnData.clear();
                columnData << QString( it->entry().name() ) << it->textStatus() << QString( "%1" ).arg( it->entry().cmtRev() ) << QString( it->entry().cmtAuthor() );
                rootItem->appendChild( new FileListItem( columnData, rootItem ) );
            }
        }
        sort ( 0, sortOrder );
        reset();
    }
}

QModelIndex FileListModel::index( int row, int column, const QModelIndex &parent ) const
{
    FileListItem *parentItem;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast< FileListItem* >( parent.internalPointer() );

    FileListItem *childItem = parentItem->child( row );
    if ( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex FileListModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    FileListItem *childItem = static_cast< FileListItem* >( index.internalPointer() );
    FileListItem *parentItem = childItem->parent();

    if ( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

int FileListModel::rowCount( const QModelIndex &parent ) const
{
    FileListItem *parentItem;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast< FileListItem* >( parent.internalPointer() );

    return parentItem->childCount();
}

int FileListModel::columnCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return static_cast< FileListItem* >( parent.internalPointer() )->columnCount();
    else
        return rootItem->columnCount();

}

QVariant FileListModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );

    return QVariant();
}

QVariant FileListModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    FileListItem *item = static_cast< FileListItem* >( index.internalPointer() );

    if ( role == Qt::DisplayRole )
        return item->data( index.column() );
    else if ( ( role == Qt::DecorationRole ) && ( index.column() == 0 ))
        return item->getPixmap();
    else
        return QVariant();
}

bool FileListModel::removeRows( int row, int count, const QModelIndex &parent )
{
    for ( int i = row; i < ( row + count ); ++i )
    {
        rootItem->removeChild( row );
    }
    return true;
}

void FileListModel::sort ( int column, Qt::SortOrder order )
{
    if ( column < 0 || column > rootItem->childCount() )
        return;

    sortOrder = order;
    int i = 0;
    //add childs to a temporary sortList with its actual row
    QVector < QPair< FileListItem*, int > > sortLst( rootItem->childCount() );
    for ( i = 0; i < rootItem->childCount(); ++i )
    {
        sortLst[ i ].first = rootItem->child( i );
        sortLst[ i ].second = i;
        sortLst[ i ].first->setSortColumn( column );
    }

    //sort the temporary list
    if ( order == Qt::AscendingOrder )
        qSort( sortLst.begin(), sortLst.end(), &itemLessThan );
    else
        qSort( sortLst.begin(), sortLst.end(), &itemGreaterThan );

    removeRows( 0, rootItem->childCount() );
    //fill sorted list into rootItem.
    for ( i = 0; i < sortLst.count(); ++i )
    {
        rootItem->appendChild( sortLst.at( i ).first );
    }
    emit layoutChanged();
}

bool FileListModel::itemLessThan( const QPair< FileListItem*, int > &left, const QPair< FileListItem*, int > &right )
{
    return ( *left.first ) < ( *right.first );
}

bool FileListModel::itemGreaterThan( const QPair< FileListItem*, int > &left, const QPair< FileListItem*, int > &right )
{
    return !( ( *left.first ) < ( *right.first ) );
}
