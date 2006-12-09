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
#include "config.h"
#include "statustext.h"
#include "wcmodel.h"

//SvnCpp
#include "svnqt/wc.hpp"


WcModel::WcModel( QObject *parent )
    : QStandardItemModel( parent )
{
    loadWcList();
}

WcModel::~WcModel()
{
    saveWcList();
}

void WcModel::addDir( QString dir, QStandardItem *parent )
{
    QStandardItem *item = new QStandardItem( QDir::cleanPath( dir ) );

    if ( svn::Wc::checkWc( dir.toLocal8Bit() ) )
        item->setIcon( QIcon( ":folder.png" ) );
    else
        item->setIcon( QIcon( ":unknownfolder.png" ) );

    item->setData( dir );
    parent->appendRow( item );

    //call addDirectory for every sub-directory
    QStringList dirLst = QDir( dir ).entryList( QDir::AllDirs );
    foreach( QString entry, dirLst )
        if ( ( entry != "." ) && ( entry != ".." ) )
            addDir(  dir + QDir::separator() + entry, item );

}

void WcModel::saveWcList()
{
    QStringList wcList;

    for ( int i = 0; i < invisibleRootItem()->rowCount(); i++ )
        wcList << invisibleRootItem()->child( i )->data().toString();

    Config::instance()->saveStringList( "workingCopies2", wcList );
}

void WcModel::loadWcList()
{
    QStringList wcList = Config::instance()->getStringList( "workingCopies" );
    wcList.sort();

    foreach ( QString wc, wcList )
        addDir( wc, invisibleRootItem() );
}
