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
#include "config.h"
#include "configure.h"
#include "filelist.h"
#include "filelistitem.h"
#include "fileselector.h"
#include "qsvn.h"
#include "statustext.h"
#include "svnclient.h"
#include "workingcopy.h"
#include "workingcopyitem.h"

//Qt
#include <qaction.h>
#include <qmessagebox.h>
#include <qtextedit.h>
#include <qurl.h>
#include <qwidgetstack.h>


QSvn::QSvn( QWidget *parent, const char *name )
        : QSvnDlg( parent, name )
{
    //insert WorkingCopy widget
    widgetStackWorkingCopy->addWidget( WorkingCopy::Exemplar()->getWidget() );
    widgetStackWorkingCopy->raiseWidget( WorkingCopy::Exemplar()->getWidget() );
    //insert FileList widget
    widgetStackFileList->addWidget( FileList::Exemplar()->getWidget() );
    widgetStackFileList->raiseWidget( FileList::Exemplar()->getWidget() );
    //insert StatusText widget
    widgetStackStatusText->addWidget( StatusText::Exemplar()->getWidget() );
    widgetStackStatusText->raiseWidget( StatusText::Exemplar()->getWidget() );
    
    Config::Exemplar()->restoreWorkingCopyEntries();
    
    //connect
    connect( WorkingCopy::Exemplar(), SIGNAL( directoryChanged( QString ) ), FileList::Exemplar(), SLOT( updateListSlot( QString ) ) );
    
    Config::Exemplar()->restoreMainWindow( this );
}

QSvn::~QSvn()
{}

//protected slots
void QSvn::exitSlot()
{
    Config::Exemplar()->saveChanges();
    Config::Exemplar()->saveMainWindow( this );
    FileList::releaseExemplar();
    WorkingCopy::releaseExemplar();
    this->close();
}

void QSvn::addWorkingCopySlot()
{
    WorkingCopy::Exemplar()->addExistingWorkingCopySlot();
}

void QSvn::removeWorkingCopySlot()
{
    WorkingCopy::Exemplar()->removeCurrentWorkingCopySlot();
}

void QSvn::checkoutSlot()
{
    WorkingCopy::Exemplar()->checkoutSlot();
}

void QSvn::updateSlot()
{
    WorkingCopyItem *item;
    
    if ( WorkingCopy::Exemplar()->getWidget()->hasFocus() && WorkingCopy::Exemplar()->selectedWorkingCopyItem())
    {
        //update a directory
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
        QUrl url( item->fullPath() );
        QString file = url.fileName();
        SvnClient::Exemplar()->update( item->fullPath() );
    } 
    else if ( FileList::Exemplar()->getWidget()->hasFocus() && WorkingCopy::Exemplar()->selectedWorkingCopyItem() )
    {
        //update file(s)
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
        SvnClient::Exemplar()->update( item->fullPath(), FileList::Exemplar()->selectedFileListItems() );
    } 
    else
    {
        return;
    }
    //Updates
    WorkingCopy::Exemplar()->updateElement( item );
    FileList::Exemplar()->updateListSlot( item->fullPath() );
}

void QSvn::commitSlot()
{
    if ( !WorkingCopy::Exemplar()->selectedWorkingCopyItem() )
        return;
    
    QString commitMessage;
    WorkingCopyItem *item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
    
    FileSelector::Exemplar()->initFileSelector( FileSelector::Commit, item->fullPath() );
    
    if ( WorkingCopy::Exemplar()->getWidget()->hasFocus() )
    {
        QStringList *fileList = new QStringList;
        SvnClient::Exemplar()->changedFilesToList( fileList, item->fullPath(), "." );
        FileSelector::Exemplar()->setSelectedFiles( fileList );
    }
    else if ( FileList::Exemplar()->getWidget()->hasFocus() )
    {
        FileSelector::Exemplar()->setSelectedFiles( FileList::Exemplar()->selectedFileListItems() );
    }    
    
    if ( FileSelector::Exemplar()->exec() )
    {
        QStringList* fileList = FileSelector::Exemplar()->selectedFiles();
        QString commitMessage = FileSelector::Exemplar()->messageString();
        SvnClient::Exemplar()->commmit( item->fullPath(), fileList, commitMessage );
    }
    //Updates
    WorkingCopy::Exemplar()->updateElement( item );
    FileList::Exemplar()->updateListSlot( item->fullPath() );
}

void QSvn::addSlot()
{
    WorkingCopyItem *item;
    
    if ( WorkingCopy::Exemplar()->getWidget()->hasFocus() && WorkingCopy::Exemplar()->selectedWorkingCopyItem() )
    {
        //add a directory
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem()->parent();
        QUrl url( WorkingCopy::Exemplar()->selectedWorkingCopyItem()->fullPath() );
        QString file = url.fileName();
        SvnClient::Exemplar()->add( item->fullPath(), file );
    } 
    else if ( FileList::Exemplar()->getWidget()->hasFocus() && WorkingCopy::Exemplar()->selectedWorkingCopyItem() )
    {
        //add file(s)
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
        SvnClient::Exemplar()->add( item->fullPath(), FileList::Exemplar()->selectedFileListItems() );
    } 
    else
    {
        return;
    }
    //Updates
    WorkingCopy::Exemplar()->updateElement( item );
    FileList::Exemplar()->updateListSlot( item->fullPath() );
}

void QSvn::removeSlot()
{
    //todo: implement
    StatusText::Exemplar()->outputMessage( QString( "svn remove - not implemented yet" ) );
}

void QSvn::revertSlot()
{
    WorkingCopyItem *item;
    
    if ( WorkingCopy::Exemplar()->getWidget()->hasFocus() && WorkingCopy::Exemplar()->selectedWorkingCopyItem() )
    {
        //revert a directory
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
        QUrl url( item->fullPath() );
        QString file = url.fileName();
        SvnClient::Exemplar()->revert( item->fullPath(), file );
    } 
    else if ( FileList::Exemplar()->getWidget()->hasFocus() && WorkingCopy::Exemplar()->selectedWorkingCopyItem() )
    {
        //revert a file
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
        SvnClient::Exemplar()->revert( item->fullPath(), FileList::Exemplar()->selectedFileListItems() );
    } 
    else
    {
        return;
    }
    
    //Updates
    WorkingCopy::Exemplar()->updateElement( item );
    FileList::Exemplar()->updateListSlot( item->fullPath() );
}

void QSvn::diffSlot()
{
    SvnClient::Exemplar()->diff( WorkingCopy::Exemplar()->selectedWorkingCopyItem()->fullPath(), FileList::Exemplar()->selectedFileListItems(), FALSE );
}

void QSvn::configureQSvnSlot()
{
    Configure configure( this );
    configure.exec();
}

void QSvn::helpContentsSlot()
{
    //todo: implement
    StatusText::Exemplar()->outputMessage( QString( "Help Content - not implemented yet" ) );
}

void QSvn::helpIndexSlot()
{
    //todo: implement
    StatusText::Exemplar()->outputMessage( QString( "Help Index - not implemented yet" ) );
}

void QSvn::aboutSlot()
{
    QMessageBox::about( this, tr( "About QSvn" ),
                        tr( "This Programm is a simple Subversion Client\n"
                            "Authors: Andreas Richter (ar@oszine.de)" ) );
}

void QSvn::aboutQtSlot()
{
    QMessageBox::aboutQt( this );
}
