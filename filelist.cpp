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

//QSvn
#include "filelist.h"

//Qt
#include <qlistview.h>
#include <qdir.h>

//make FileList a singleton
FileList* FileList::_exemplar = 0;

FileList* FileList::Exemplar()
{
    if ( _exemplar == 0 )
    {
        _exemplar = new FileList;
    }
    return _exemplar;
}

//FileList implementation
FileList::FileList(QObject *parent, const char *name)
        : QObject(parent, name)
{
    listViewFiles = new QListView( 0, "listViewFiles" );
    listViewFiles->addColumn( tr( "Filename" ) );
    listViewFiles->addColumn( tr( "Extension" ) );
    listViewFiles->addColumn( tr( "State" ) );
}

FileList::~FileList()
{}

QWidget *FileList::getWidget()
{
    return listViewFiles;
}

void FileList::updateListSlot( QString stringDirectory )
{
    if ( listViewFiles )
    {
        listViewFiles->clear();
        QDir directory( stringDirectory );
        QStringList listFiles = directory.entryList( QDir::Files );
        for ( QStringList::Iterator it = listFiles.begin(); it != listFiles.end(); ++it ) 
        {
            // add only directories here
            if ( ( *it != "." ) && ( *it != ".." ) )
            {
                QListViewItem* _element = new QListViewItem( listViewFiles, *it );
                _element->setText( 2, "?" );
            }
        }
    }
}
