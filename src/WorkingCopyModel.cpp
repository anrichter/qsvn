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
#include "Config.h"
#include "WorkingCopyItem.h"
#include "WorkingCopyModel.h"

//Qt
#include <QDir>
#include <QString>
#include <QtGui>


WorkingCopyModel::WorkingCopyModel( QObject *parent )
    : QAbstractItemModel( parent )
{
    QList< QVariant > rootData;
    rootData << "Working Copy";
    rootItem = new WorkingCopyItem( rootData );
    
    loadWorkingCopies();
}

WorkingCopyModel::~WorkingCopyModel()
{
    saveWorkingCopies();
    delete rootItem;
}

void WorkingCopyModel::addWorkingCopy( QString directory )
{
    if ( directory.isNull() )
        return;

    QList< QVariant > columnData;

    columnData << directory << directory;
    WorkingCopyItem *item = new WorkingCopyItem( columnData, rootItem );
    rootItem->appendChild( item );
    updateWorkingCopy( item );
    emit layoutChanged();
}

void WorkingCopyModel::updateWorkingCopy( WorkingCopyItem *item )
{
    if ( !item )
        return;

    item->deleteAllChilds();

    QDir directory( item->data( 1 ).toString() );
    if ( directory.exists() )
    {
        //add new child items
        QStringList lst = directory.entryList( QDir::AllDirs );
        for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
        {
            // add only directories here
            if ( ( *it != "." ) && ( *it != ".." ) )
            {
                QList< QVariant > columnData;
                columnData << *it << item->data( 1 ).toString() + QDir::separator() +  *it;
                WorkingCopyItem *newItem = new WorkingCopyItem( columnData, item );
                item->appendChild( newItem );

                updateWorkingCopy( newItem );
            }
        }

    }
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

QVariant WorkingCopyModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );

    return QVariant();
}

QVariant WorkingCopyModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    WorkingCopyItem *item = static_cast< WorkingCopyItem* >( index.internalPointer() );

    if ( role == Qt::DisplayRole )
        return item->data( index.column() );
    else if ( role == Qt::DecorationRole && index.column() == 0 )
        return item->getPixmap();
    else
        return QVariant();
        
    
}

void WorkingCopyModel::saveWorkingCopies()
{
	QStringList wcList;
	
	for ( int i = 0; i < rootItem->childCount(); ++i )
	{
		wcList << rootItem->child( i )->data( 1 ).toString();
	}
	Config::Exemplar()->setWorkingCopies( &wcList );
}

void WorkingCopyModel::loadWorkingCopies()
{
    QStringList wcList = Config::Exemplar()->getWorkingCopies();

	for ( int i = 0; i < wcList.size(); i++ )
	{
		addWorkingCopy( wcList.at( i ) );
	}
}
