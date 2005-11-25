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
 ***************************************************************************/


//QSvn
#include "filelistitem.h"
#include "filelistmodel.h"
#include "statustext.h"
#include "svnclient.h"
#include "workingcopyitem.h"

//Qt
#include <QtGui>


FileListModel::FileListModel( QObject *parent )
        : QAbstractItemModel( parent )
{
    m_modelFor = None;
    initModel();
}

FileListModel::FileListModel( QObject *parent, ModelFor modelFor )
    : QAbstractItemModel( parent )
{
    m_modelFor = modelFor;
    initModel();
}

FileListModel::~FileListModel()
{
    delete rootItem;
}

void FileListModel::initModel( )
{
    QList< QVariant > rootData;
    rootData << "Filename" << "Status" << "Revision" << "Author";
    rootItem = new FileListItem( rootData );
    sortOrder = Qt::AscendingOrder;
}

bool FileListModel::isStatusForModel( svn_wc_status_kind status )
{
    switch ( m_modelFor )
    {
        case None:
            return true;
            break;
        case Add:
            if ( status == svn_wc_status_unversioned )
                return true;
            break;
        case Commit:
            if ( ( status == svn_wc_status_modified ) ||
                   ( status == svn_wc_status_added ) ||
                   ( status == svn_wc_status_deleted ) ||
                   ( status == svn_wc_status_replaced ) )
                return true;
            break;
        case Remove:
            if ( ( status == svn_wc_status_normal ) ||
                   ( status == svn_wc_status_merged ) )
                return true;
            break;
        case Revert:
            if ( ( status == svn_wc_status_modified ) ||
                   ( status == svn_wc_status_added ) ||
                   ( status == svn_wc_status_deleted ) ||
                   ( status == svn_wc_status_replaced ) )
                return true;
            break;
    }
    return false;
}

void FileListModel::loadFromWorkingCopySelection( QItemSelectionModel *itemSelection )
{
    QModelIndexList indexes = itemSelection->selectedIndexes();
    if ( indexes.count() > 0 )
    {
        WorkingCopyItem *item = static_cast< WorkingCopyItem* >( indexes.at( 0 ).internalPointer() );
        QStringList fullPathList = item->fullPathListRecursive();
        QString fullPath;
        QString fileNamePrefix;

        foreach( fullPath, fullPathList )
        {
            fileNamePrefix = fullPath;
            fileNamePrefix.remove( 0, item->fullPath().length() + 1 );
            if ( ( fileNamePrefix.count() > 0 ) && !fileNamePrefix.endsWith( QDir::separator(), Qt::CaseInsensitive ) )
                fileNamePrefix.append( QDir::separator() );

            loadFromDirectory( fullPath, fileNamePrefix );
        }
    }
    sort ( FileListItem::FilenameColumn, sortOrder );
    reset();
}

void FileListModel::loadFromFileListSelection( QItemSelectionModel *itemSelection )
{
    QSet< FileListItem* > fileListItemSet;
    QModelIndexList indexes = itemSelection->selectedIndexes();
    int row = -1;
    FileListItem *item = 0;

    for ( int i = 0; i < indexes.count(); i++ )
    {
        if ( row != indexes.at( i ).row() ) //new row - load new item
        {
            if ( item && !fileListItemSet.contains( item ) && isStatusForModel( svn_wc_status_kind( item->data( 1 ).toInt() ) ) ) // add already loaded item to rootItem
            {
                rootItem->appendChild( new FileListItem( item, rootItem ) );
                fileListItemSet << item;
            }

            row = indexes.at( i ).row();
            item = static_cast< FileListItem* >( indexes.at( i ).internalPointer() );
        }
    }
    if ( item && !fileListItemSet.contains( item ) && isStatusForModel( svn_wc_status_kind( item->data( 1 ).toInt() ) ) )
    {
        rootItem->appendChild( new FileListItem( item, rootItem ) );
    }
    sort ( FileListItem::FilenameColumn, sortOrder );
    reset();
}

void FileListModel::loadFromDirectory( QString directory, QString fileNamePrefix )
{
    if ( !directory.isEmpty() )
    {
        QList< QVariant > columnData;
        QFileInfo fileInfo;
        bool visible; 

        svn::StatusEntries statusList = SvnClient::instance()->status( directory );
        svn::StatusEntries::iterator it;

        for ( it = statusList.begin(); it != statusList.end(); it++ )
        {
            fileInfo = QFileInfo( it->path() );
            if ( m_modelFor == FileListModel::None )
            {
                visible = !fileInfo.isDir();
            }
            else
            {
                if ( fileInfo.isDir() )
                    visible = ( fileInfo.canonicalPath() != QFileInfo( directory ).canonicalPath() ) && 
                              isStatusForModel( it->textStatus() );
                else
                    visible = isStatusForModel( it->textStatus() );
            }
            
            if (  visible )
            {
                columnData.clear();
                if ( it->isVersioned() )
                    columnData << fileNamePrefix + it->entry().name();
                else
                {
                    QFileInfo fileInfo( it->path() );
                    columnData << fileNamePrefix + fileInfo.fileName();
                }
                columnData << it->textStatus() << int ( it->entry().cmtRev() ) << it->entry().cmtAuthor() << it->path();
                rootItem->appendChild( new FileListItem( columnData, rootItem ) );
            }
        }
        sort ( FileListItem::FilenameColumn, sortOrder );
    }
    reset();
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
    {
        if ( index.column() == FileListItem::StatusColumn )
        {
            int statusKind = item->data( FileListItem::StatusColumn ).toInt();
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
                    return item->data( FileListItem::StatusColumn ).toString();
            }
        }
        else
        {
            return item->data( index.column() );
        }
    }
    else if ( ( role == Qt::DecorationRole ) && ( index.column() == FileListItem::FilenameColumn ))
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

void FileListModel::removeAllRows()
{
    removeRows( 0, rootItem->childCount() );
    reset();
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
