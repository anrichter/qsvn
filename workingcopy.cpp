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
    statusTextEdit = 0;
    workingCopyListView = 0;
}

WorkingCopy::~WorkingCopy()
{}

void WorkingCopy::outputMessage( const QString messageString )
{
    if ( statusTextEdit )
        statusTextEdit->append( messageString );
    else
        qDebug( messageString );
}

void WorkingCopy::setListView( QListView *listView )
{
    workingCopyListView = listView;
}

void WorkingCopy::setStatusEdit( QTextEdit *textEdit )
{
    statusTextEdit = textEdit;
}

void WorkingCopy::addWorkingCopySlot()
{
    if ( !addWorkingCopy )
    {
        addWorkingCopy = new AddWorkingCopy();
    }

    if ( addWorkingCopy->exec() && SvnClient::Exemplar()->isWorkingCopy( addWorkingCopy->getSelectedDirectory() ) )
    {
        //ad working Copy to workingCopyListView
        QListViewItem *element;
        element = new QListViewItem( workingCopyListView, addWorkingCopy->getSelectedDirectory() );
        updateElement( element );
    }
    else
    {
        statusTextEdit->append( SvnClient::Exemplar()->getProcessStderr() );
        statusTextEdit->append( SvnClient::Exemplar()->getMessageString() );
    }
}

void WorkingCopy::updateElement( QListViewItem *element )
{
    if ( &element )
    {
        //todo: delete old childs
        QDir directory( element->text( 0 ) );
        if ( directory.exists() )
        {
            directory.setMatchAllDirs( TRUE );
            QStringList lst = directory.entryList( "*" );
            for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it ) 
            {
                //todo: check if is dir & call updateElemtent recursively for directories
                // add only directories here
                QListViewItem *_element;
                _element = new QListViewItem( element, *it );
            }
        }
    }
}
