/***************************************************************************
 *   This file is part of QSvn Project http://ar.oszine.de/projects/qsvn   *
 *   Copyright (c) 2004-2007 Andreas Richter <ar@oszine.de>                *
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
#include "config.h"
#include "statustext.h"
#include "wcmodel.h"

//SvnCpp
#include "svnqt/wc.hpp"

//Qt
#include <QtGui>


WcModel::WcModel( QObject *parent )
    : QStandardItemModel( parent )
{
    setHorizontalHeaderLabels( QStringList( "Working Copy" ) );
    loadWcList();
}

WcModel::~WcModel()
{
    saveWcList();
}

int WcModel::rowCount( const QModelIndex &parent ) const
{
    if ( !parent.isValid() )
        return QStandardItemModel::rowCount( parent );

    const WcModel *model = static_cast<const WcModel *>( parent.model() );
    QStandardItem *item = model->itemFromIndex( parent );
    if ( item->rowCount() == 0 )
    {
        //add subDirectories
        foreach( QString entry, QDir( model->getPath( parent ) ).entryList( QDir::AllDirs ) )
            if ( ( entry != "." ) && ( entry != ".." ) )
                model->addDir( entry, item );
    }

    return item->rowCount();
}

bool WcModel::hasChildren( const QModelIndex &parent ) const
{
    return rowCount( parent ) > 0;
}

void WcModel::addWc( QString dir )
{
    addDir( dir, invisibleRootItem() );
}

void WcModel::removeWc( const QModelIndex &index )
{
    removeRow( index.row(), index.parent() );
}

QString WcModel::getPath( const QModelIndex &index ) const
{
    return itemFromIndex( index )->data().toString();
}

void WcModel::addDir( QString dir, QStandardItem *parent ) const
{
    QStandardItem *item = new QStandardItem();

    item->setText( QDir::toNativeSeparators( dir ) );

    //complete dir to full path if necessary
    if ( parent != invisibleRootItem() )
        dir = parent->data().toString() + QDir::separator() + dir;
    item->setData( QDir::toNativeSeparators( dir ) );

    if ( svn::Wc::checkWc( dir.toLocal8Bit() ) )
        item->setIcon( QIcon( ":/images/folder.png" ) );
    else
        item->setIcon( QIcon( ":/images/unknownfolder.png" ) );

    parent->appendRow( item );
}

void WcModel::saveWcList()
{
    QStringList wcList;

    for ( int i = 0; i < invisibleRootItem()->rowCount(); i++ )
        wcList << invisibleRootItem()->child( i )->data().toString();

    Config::instance()->saveStringList( "workingCopies", wcList );
}

void WcModel::loadWcList()
{
    QStringList wcList = Config::instance()->getStringList( "workingCopies" );
    wcList.sort();

    foreach ( QString wc, wcList )
        addDir( QDir::cleanPath( wc ), invisibleRootItem() );
}

void WcModel::doUpdate( const QModelIndex &index )
{
    QStandardItem *item = itemFromIndex( index );
    item->removeRows( 0, item->rowCount() );
}

#include "wcmodel.moc"
