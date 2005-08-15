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
#include "svnwrapper.h"

//Qt
#include <QtGui>


FileListModel::FileListModel( QObject *parent )
        : QAbstractItemModel( parent )
{
    QList< QVariant > rootData;
    rootData << "Filename" << "Status" << "Revision" << "Author";
    rootItem = new FileListItem( rootData );
}

FileListModel::~FileListModel()
{
    delete rootItem;
}

void FileListModel::setActiveDirectory( QString directory )
{
    rootItem->deleteAllChilds();

    if ( directory.isEmpty()   || !( SvnWrapper::Exemplar()->doSvnCommand( SvnWrapper::Status, directory, false ) ) )
        return;

    QStringList statusList( SvnWrapper::Exemplar()->getProcessStdoutList() );
    QString _lineString, _restString, _status, _revision, _author, _fileName;
    for ( QStringList::Iterator it = statusList.begin(); it != statusList.end(); ++it )
    {
        _lineString = *it;
        _restString = _lineString.right( _lineString.length() - 6 );
        _restString = _restString.simplified(); //convert into simple whitespace seaparatet string
        if ( _lineString.at( 0 ) == '?' )
        {
            _status = "";
            _revision = "";
            _author = "";
            _fileName = _restString;
        }
        else
        {
            _status = _restString.section( ' ', 0, 0 );
            _revision = _restString.section( ' ', 1, 1 );
            _author = _restString.section( ' ', 2, 2 );
            _fileName = _restString.section( ' ', 3, 3 );
        }
        // add only files here
        if ( ! QDir( directory + QDir::separator() + _fileName ).exists() )
        {
            //set Filename
            QList< QVariant > columnData;
            columnData << _fileName << _status << _revision << _author;
            FileListItem *item = new FileListItem( columnData, rootItem );
            rootItem->appendChild( item );
            emit layoutChanged();
        }
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

    if ( role != Qt::DisplayRole )
        return QVariant();

    FileListItem *item = static_cast< FileListItem* >( index.internalPointer() );

    return item->data( index.column() );
}

