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
    WorkingCopy::Exemplar()->updateCurrentWorkingCopySlot();
}

void QSvn::commitSlot()
{
    //todo: implement
    StatusText::Exemplar()->outputMessage( QString( "svn commit - not implemented yet" ) );
}

void QSvn::addSlot()
{
    QString path;
    QString file;
    WorkingCopyItem *item;
    
    if ( WorkingCopy::Exemplar()->getWidget()->hasFocus() )
    {
        //add a directory
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem()->parent();
        path = item->fullPath();
        QUrl url( WorkingCopy::Exemplar()->selectedWorkingCopyItem()->fullPath() );
        file = url.fileName();
    } 
    else if ( FileList::Exemplar()->getWidget()->hasFocus() )
    {
        //add a file
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
        path = item->fullPath();
        file = FileList::Exemplar()->selectedFileListItem()->text( _COLUMN_FILE );
    } 
    else
    {
        return;
    }
    
    //Add
    SvnClient::Exemplar()->add( path, file );
    
    //Updates
    WorkingCopy::Exemplar()->updateElement( item );
    FileList::Exemplar()->updateListSlot( WorkingCopy::Exemplar()->selectedWorkingCopyItem()->fullPath() );
}

void QSvn::removeSlot()
{
    //todo: implement
    StatusText::Exemplar()->outputMessage( QString( "svn remove - not implemented yet" ) );
}

void QSvn::revertSlot()
{
    QString path;
    QString file;
    WorkingCopyItem *item;
    
    if ( WorkingCopy::Exemplar()->getWidget()->hasFocus() )
    {
        //revert a directory
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem()->parent();
        path = item->fullPath();
        QUrl url( WorkingCopy::Exemplar()->selectedWorkingCopyItem()->fullPath() );
        file = url.fileName();
    } 
    else if ( FileList::Exemplar()->getWidget()->hasFocus() )
    {
        //revert a file
        item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
        path = item->fullPath();
        file = FileList::Exemplar()->selectedFileListItem()->text( _COLUMN_FILE );
    } 
    else
    {
        return;
    }
    
    //Add
    SvnClient::Exemplar()->revert( path, file );
    
    //Updates
    WorkingCopy::Exemplar()->updateElement( item );
    FileList::Exemplar()->updateListSlot( WorkingCopy::Exemplar()->selectedWorkingCopyItem()->fullPath() );
}

void QSvn::diffSlot()
{
    FileList::Exemplar()->diffSelected();
}

void QSvn::configureQSvnSlot()
{
    Configure configure;
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
