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
#include "fileselector.h"
#include "fileselectordlg.h"
#include "svnclient.h"

//Qt
#include <qapplication.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qdir.h>
#include <qlistview.h>
#include <qsplitter.h>
#include <qstringlist.h>
#include <qtextedit.h>


//make FileSelector a singleton
FileSelector* FileSelector::_exemplar = 0;

FileSelector* FileSelector::Exemplar()
{
    if ( _exemplar == 0 )
    {
        _exemplar = new FileSelector( qApp->mainWidget() );
    }
    return _exemplar;
}

void FileSelector::releaseExemplar()
{
    delete _exemplar;
}


//FileSelector implementation
FileSelector::FileSelector( QWidget *parent, const char *name )
        : FileSelectorDlg( parent, name )
{
    QValueList<int> list = splitterVertical->sizes();
    logMessageSize = *list.at( 1 );

    //coonections
    connect( comboLogMessage, SIGNAL( activated( const QString & ) ),
             this, SLOT( comboLogMessageActivatedSlot( const QString & ) ) );
}

FileSelector::~FileSelector()
{}

void FileSelector::showLogMessage()
{
    frameLogMessage->setEnabled( TRUE );
    QValueList<int> list = splitterVertical->sizes();
    *list.at( 1 ) = logMessageSize;
    splitterVertical->setSizes( list );
}

void FileSelector::hideLogMessage()
{
    QValueList<int> list = splitterVertical->sizes();
    if ( *list.at( 1 ) > 0 )
    {
        logMessageSize = *list.at( 1 );
        *list.at( 0 ) += *list.at( 1 );
        *list.at( 1 ) = 0;
        splitterVertical->setSizes( list );
        frameLogMessage->setEnabled( FALSE );
    }
}

void FileSelector::initFileSelector( int svnCommandType, const QString &path )
{
    switch ( svnCommandType )
    {
    case SvnClient::Add:
        setCaption( tr( "Add") );
        hideLogMessage();
        break;
    case SvnClient::Commit:
        setCaption( tr( "Commit") );
        showLogMessage();
        break;
    case SvnClient::Remove:
        setCaption( tr( "Remove") );
        hideLogMessage();
        break;
    case SvnClient::Revert:
        setCaption( tr( "Revert") );
        hideLogMessage();
        break;
    }
    listViewFiles->clear();
    setMessageString( tr( "***empty message ***" ) );
    startPath = path;
    _svnCommandType = svnCommandType;
}

void FileSelector::setSelectedFiles( QStringList* fileList )
{
    listViewFiles->clear();

    if ( fileList )
    {
        for ( QStringList::Iterator it = fileList->begin(); it != fileList->end(); ++it )
        {
            QCheckListItem* _item = new QCheckListItem( listViewFiles, *it, QCheckListItem::CheckBox );
            if ( _svnCommandType == SvnClient::Remove )
            {
                _item->setOn( FALSE );
            }
            else
            {
                _item->setOn( TRUE );
            }
        }
    }
}

void FileSelector::setMessageString( QString message )
{
    editLogMessage->setText( message );
}

QStringList* FileSelector::selectedFiles()
{
    QStringList *stringList = new QStringList;

    QListViewItemIterator it( listViewFiles );
    while ( it.current() )
    {
        QCheckListItem *item = static_cast< QCheckListItem* >( it.current() );
        if ( item->isOn() )
        {
            stringList->append( item->text( 0 ) );
        }
        ++it;
    }
    return stringList;
}

QString FileSelector::messageString()
{
    return editLogMessage->text();
}

void FileSelector::listViewFilesDoubleClickSlot()
{
    SvnClient::Exemplar()->diff( startPath + QDir::separator() + listViewFiles->selectedItem()->text( 0 ), FALSE );
}

void FileSelector::buttonOkClickSlot()
{
    if ( _svnCommandType == SvnClient::Commit )
    {
        comboLogMessage->insertItem( editLogMessage->text(), 0 );
    }
    this->accept();
}

void FileSelector::comboLogMessageActivatedSlot( const QString& selectedString )
{
    setMessageString( selectedString );
}
