/***************************************************************************
 *   Copyright (C) 2004 by Andreas Richter                                 *
 *   ar@oszine.de                                                          *
 *   http://www.oszine.de                                                  *
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

//qsvn
#include "workingcopy.h"
#include "addworkingcopy.h"
#include "svnclient.h"
#include "statustext.h"

//Qt
#include <qlistview.h>
#include <qtextedit.h>
#include <qdir.h>

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

//WorkingCopy implementation
WorkingCopy::WorkingCopy(QObject *parent, const char *name)
        : QObject(parent, name)
{
    //initialising
    addWorkingCopy = 0;

    listViewWorkingCopy = new QListView( 0, "listViewWorkingCopy" );
    listViewWorkingCopy->addColumn( tr( "Working Copy" ) );
    listViewWorkingCopy->setRootIsDecorated( TRUE );

    connect( listViewWorkingCopy, SIGNAL( selectionChanged() ), this, SLOT( changeElement() ) );
}

WorkingCopy::~WorkingCopy()
{}

QWidget *WorkingCopy::getWidget()
{
    return listViewWorkingCopy;
}

void WorkingCopy::updateElement( QListViewItem *element, QString directoryString )
{
    if ( &element && directoryString )
    {
        //todo: delete old childs
        QDir directory( directoryString );
        if ( directory.exists() )
        {
            directory.setMatchAllDirs( TRUE );
            QStringList lst = directory.entryList( QDir::Dirs );
            for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
            {
                // add only directories here
                if ( ( *it != "." ) && ( *it != ".." ) )
                {
                    QListViewItem *_element;
                    _element = new QListViewItem( element, *it );
                    // recursive call for new _element
                    this->updateElement( _element, directoryString + QDir::separator() + *it );
                }
            }
        }
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
        //ad working Copy to workingCopyListView
        QListViewItem *element;
        element = new QListViewItem( listViewWorkingCopy, addWorkingCopy->getSelectedDirectory() );
        updateElement( element, addWorkingCopy->getSelectedDirectory() );
    }
    else
    {
        StatusText::Exemplar()->outputMessage( SvnClient::Exemplar()->getProcessStderr() );
        StatusText::Exemplar()->outputMessage( SvnClient::Exemplar()->getMessageString() );
    }
}

void WorkingCopy::removeCurrentWorkingCopySlot()
{
    if ( listViewWorkingCopy->currentItem() )
        removeWorkingCopy( listViewWorkingCopy->currentItem() );
}

void WorkingCopy::removeWorkingCopy( QListViewItem *element )
{
    if ( element )
    {
        if ( element->parent() )
        {
            this->removeWorkingCopy( element->parent() );
        }
        else
        {
            listViewWorkingCopy->removeItem( element );
        }
        emit directoryChanged( QString( "" ) );
    }
}

QString WorkingCopy::getFullDirectory( QListViewItem *element )
{
    QString strDirectory;
    if ( element )
    {
        if ( element->parent() )
        {
            strDirectory = this->getFullDirectory( element->parent() ) + QDir::separator() + element->text( 0 );
        }
        else
        {
            strDirectory = element->text( 0 );
        }
    }
    return strDirectory;
}

//private slots
void WorkingCopy::changeElement()
{
    emit directoryChanged( this->getFullDirectory( listViewWorkingCopy->selectedItem() ) );
}
