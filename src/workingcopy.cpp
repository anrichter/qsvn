/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2005 Andreas Richter <ar@oszine.de>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
#include "addworkingcopy.h"
#include "checkout.h"
#include "config.h"
#include "statustext.h"
#include "svnclient.h"
#include "workingcopy.h"
#include "workingcopyitem.h"

//Qt
#include <qdir.h>
#include <qlistview.h>
#include <qpixmap.h>
#include <qtextedit.h>
#include <qurl.h>


//make WorkingCopy a singleton
WorkingCopy* WorkingCopy::_exemplar = 0;

WorkingCopy* WorkingCopy::Exemplar()
{
    if ( _exemplar == 0 )
    {
        _exemplar = new WorkingCopy;
    }
    return _exemplar;
}

void WorkingCopy::releaseExemplar()
{
    delete _exemplar;
}


//WorkingCopy implementation
WorkingCopy::WorkingCopy(QObject *parent, const char *name)
        : QObject(parent, name)
{
    //initialising
    addWorkingCopy = 0;
    checkout = 0;

    listViewWorkingCopy = new QListView( 0, "listViewWorkingCopy" );
    listViewWorkingCopy->addColumn( tr( "Working Copy" ) );
    listViewWorkingCopy->setRootIsDecorated( TRUE );
    
    Config::Exemplar()->restoreListView( listViewWorkingCopy );

    connect( listViewWorkingCopy, SIGNAL( selectionChanged() ), this, SLOT( changeElement() ) );
}

WorkingCopy::~WorkingCopy()
{
    Config::Exemplar()->saveWorkingCopyEntries( listViewWorkingCopy );
    Config::Exemplar()->saveListView( listViewWorkingCopy );
}

QWidget *WorkingCopy::getWidget()
{
    return listViewWorkingCopy;
}

void WorkingCopy::updateElement( WorkingCopyItem *element )
{
    if ( &element )
    {
        QDir directory( element->fullPath() );
        if ( directory.exists() )
        {
            //delete all client items
            while ( element->childCount() > 0 )
            {
                delete element->firstChild();
            }

            //add new child items
            directory.setMatchAllDirs( TRUE );
            QStringList lst = directory.entryList( QDir::Dirs );
            for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
            {
                // add only directories here
                if ( ( *it != "." ) && ( *it != ".." ) )
                {
                    WorkingCopyItem *newelement;
                    newelement = new WorkingCopyItem( element, directory.absPath() + QDir::separator() + *it );
                    if ( newelement->isSvnDirectory() )
                        newelement->setPixmap( 0, QPixmap::fromMimeSource( "folder.png" ) );
                    else
                        newelement->setPixmap( 0, QPixmap::fromMimeSource( "unknownfolder.png" ) );
                    // recursive call for new _element
                    updateElement( newelement );
                }
            }
        }
    }
}

void WorkingCopy::addExistingWorkingCopySlot( QString directoyString )
{
    if ( directoyString )
    {
        WorkingCopyItem *element;
        element = new WorkingCopyItem( listViewWorkingCopy, directoyString );
        if ( element->isSvnDirectory() )
            element->setPixmap( 0, QPixmap::fromMimeSource( "folder.png" ) );
        else
            element->setPixmap( 0, QPixmap::fromMimeSource( "unknownfolder.png" ) );
        updateElement( element );
    }
}

void WorkingCopy::addExistingWorkingCopySlot()
{
    if ( !addWorkingCopy )
    {
        addWorkingCopy = new AddWorkingCopy();
    }

    if ( addWorkingCopy->exec() && SvnClient::Exemplar()->isWorkingCopy( addWorkingCopy->getSelectedDirectory() ) )
    {
        //add working Copy to workingCopyListView
        addExistingWorkingCopySlot( addWorkingCopy->getSelectedDirectory() );
    }
    else
    {
        StatusText::Exemplar()->outputMessage( SvnClient::Exemplar()->getProcessStderrList() );
        StatusText::Exemplar()->outputMessage( SvnClient::Exemplar()->getMessageString() );
    }
}

void WorkingCopy::checkoutSlot()
{
    if ( !checkout )
    {
        checkout = new Checkout();
    }
    
    if ( checkout->exec() )
    {
        QString path = checkout->getSelectedDirectory();
        QString url = checkout->getSelectedURL();
        
        if ( SvnClient::Exemplar()->checkout( path, url ) )
        {
            QUrl svnurl( url );
            addExistingWorkingCopySlot( QDir::cleanDirPath( path + QDir::separator() + svnurl.fileName() ) );
        }
    }
}

void WorkingCopy::removeCurrentWorkingCopySlot()
{
    if ( selectedWorkingCopyItem() )
        removeWorkingCopy( selectedWorkingCopyItem() );
}

void WorkingCopy::removeWorkingCopy( WorkingCopyItem *element )
{
    if ( element )
    {
        if ( element->parent() )
        {
            removeWorkingCopy( element->parent() );
        }
        else
        {
            listViewWorkingCopy->removeItem( element );
        }
        emit directoryChanged( QString( "" ) );
    }
}

WorkingCopyItem* WorkingCopy::selectedWorkingCopyItem()
{
    if ( listViewWorkingCopy->selectedItem() )
        return static_cast< WorkingCopyItem* >( listViewWorkingCopy->selectedItem() );
    else
        return 0;
}

//private slots
void WorkingCopy::changeElement()
{
    if ( selectedWorkingCopyItem() )
        emit directoryChanged( selectedWorkingCopyItem()->fullPath() );
}
