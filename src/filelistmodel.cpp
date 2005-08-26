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

#ifdef Q_WS_WIN
#include "svnwrapper.h"
#endif

#ifdef Q_WS_X11
//SvnCpp
#include "svncpp/client.hpp"
#include "svncpp/status.hpp"
#endif

//Qt
#include <QtGui>


FileListModel::FileListModel( QObject *parent )
        : QAbstractItemModel( parent )
{
    QList< QVariant > rootData;
    rootData << "Filename" << "Status" << "Revision" << "Author";
    rootItem = new FileListItem( rootData );
    sortOrder = Qt::AscendingOrder;

#ifdef Q_WS_X11
    svnContext = 0;
#endif
}

FileListModel::~FileListModel()
{
    delete rootItem;
#ifdef Q_WS_X11
    if ( svnContext )
    {
        delete svnContext;
        svnContext = 0;
    }
#endif
}

void FileListModel::setActiveDirectory( QString directory )
{
    removeRows( 0, rootItem->childCount() );

    if ( directory.isEmpty() )
        return;

    QList< QVariant > columnData;

#ifdef Q_WS_X11
   if ( oldDirectory != directory )
    {
        //create a new svn::Context
        if ( svnContext )
        {
            delete svnContext;
            svnContext = 0;
        }

        svnContext = new svn::Context();
        oldDirectory = directory;
    }

    svn::Client svnClient( svnContext );

    svn::StatusEntries statusList = svnClient.status( directory.toLocal8Bit(), false, true, false, false);
    svn::StatusEntries::iterator it;
    for ( it = statusList.begin(); it != statusList.end(); ++it )
    {
        if ( ! QDir( directory + QDir::separator() + QString( it->entry().name() ) ).exists() )
        {
            columnData.clear();
            //todo: add it->textStatus() to columnData
            columnData << QString( it->entry().name() ) << QString( " " ) << QString( "%1" ).arg( it->entry().revision() ) << QString( it->entry().cmtAuthor() );
            rootItem->appendChild( new FileListItem( columnData, rootItem ) );
        }
    }
#endif
#ifdef Q_WS_WIN
    if (  !( SvnWrapper::Exemplar()->doSvnCommand( SvnWrapper::Status, directory, false ) ) )
        return;

    QStringList statusList( SvnWrapper::Exemplar()->getProcessStdoutList() );
    QString _lineString, _restString, _status, _revision, _author, _fileName;

    for ( QStringList::const_iterator it = statusList.constBegin(); it != statusList.constEnd(); ++it )
    {
        _lineString = *it;

        if ( !_lineString.isEmpty() )
        {
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
                _status = _lineString.at( 0 );
                _revision = _restString.section( ' ', 1, 1 );
                _author = _restString.section( ' ', 2, 2 );
                _fileName = _restString.section( ' ', 3, 3 );
            }
            // add only files here
            if ( ! QDir( directory + QDir::separator() + _fileName ).exists() )
            {
                columnData.clear();
                columnData << _fileName << _status << _revision << _author;
                rootItem->appendChild( new FileListItem( columnData, rootItem ) );
            }
        }
    }
#endif
    sort ( 0, sortOrder );
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

    if ( role != Qt::DisplayRole )
        return QVariant();

    FileListItem *item = static_cast< FileListItem* >( index.internalPointer() );

    return item->data( index.column() );
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
    return ( left.first->data( left.first->getSortColumn() ).toString() ) < ( right.first->data( right.first->getSortColumn() ).toString() );
}

bool FileListModel::itemGreaterThan( const QPair< FileListItem*, int > &left, const QPair< FileListItem*, int > &right )
{
    return ( left.first->data( left.first->getSortColumn() ).toString() ) > ( right.first->data( right.first->getSortColumn() ).toString() );
}
