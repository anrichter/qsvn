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
#include "WorkingCopyItem.h"
#include "WorkingCopyModel.h"

//Qt
#include <QtGui>


WorkingCopyModel::WorkingCopyModel( QObject *parent )
    : QAbstractItemModel( parent )
{
    QList< QVariant > rootData;
    rootData << "Working Copy";
    rootItem = new WorkingCopyItem( rootData );
}

WorkingCopyModel::~WorkingCopyModel()
{
    delete rootItem;
}

QModelIndex WorkingCopyModel::index( int row, int column, const QModelIndex &parent ) const
{
    WorkingCopyItem *parentItem;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast< WorkingCopyItem* >( parent.internalPointer() );

    WorkingCopyItem *childItem = parentItem->child( row );
    if ( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex WorkingCopyModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    WorkingCopyItem *childItem = static_cast< WorkingCopyItem* >( index.internalPointer() );
    WorkingCopyItem *parentItem = childItem->parent();

    if ( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

int WorkingCopyModel::rowCount( const QModelIndex &parent ) const
{
    WorkingCopyItem *parentItem;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast< WorkingCopyItem* >( parent.internalPointer() );

    return parentItem->childCount();
}

int WorkingCopyModel::columnCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return static_cast< WorkingCopyItem* >( parent.internalPointer() )->columnCount();
    else
        return rootItem->columnCount();

}

QVariant WorkingCopyModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( role != Qt::DisplayRole )
        return QVariant();

    WorkingCopyItem *item = static_cast< WorkingCopyItem* >( index.internalPointer() );

    return item->data( index.column() );
}
