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
#include "QSvn.h"

/*todo:
#include "AboutDlg.h"
#include "Config.h"
#include "Configure.h"
#include "FileList.h"
#include "FileListItem.h"
#include "FileSelector.h"
#include "StatusText.h"
#include "SvnWrapper.h"
#include "WorkingCopy.h"
#include "WorkingCopyItem.h"
*/

//Qt
#include <QtGui>

QSvn::QSvn( QWidget *parent, Qt::WFlags flags )
        : QMainWindow( parent, flags )
{

    setupUi( this );

    createActions();
    createMenus();
    createToolBar();

    /*todo:
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
    */
}

void QSvn::createActions()
{
    qDebug() << "Create Actions...";
    actionExit = new QAction( "actionExit", this );
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( exitSlot() ) );

    actionAddWorkingCopy = new QAction( "actionAddWorkingCopy", this );
    actionRemoveWorkingCopy = new QAction( "actionRemoveWorkingCopy", this );
    actionCheckout = new QAction( "actionCheckout", this );

    actionHelpContents = new QAction( "actionHelpContents", this );
    actionHelpIndex = new QAction( "actionHelpIndex", this );
    actionAboutQSvn = new QAction( "actionAboutQSvn", this );

    actionUpdate = new QAction( "actionUpdate", this );
    actionCommit = new QAction( "actionCommit", this );
    actionAdd = new QAction( "actionAdd", this );
    actionRemove = new QAction( "actionRemove", this );
    actionRevert = new QAction( "actionRevert", this );

    actionConfigureQSvn = new QAction( "actionConfigureQSvn", this );

    actionAboutQt = new QAction( "actionAboutQt", this );
    connect(actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

    actionDiff = new QAction( "actionDiff", this );
}

void QSvn::createMenus()
{
    qDebug() << "create Menus";

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(actionExit);

    workingCopyMenu = menuBar()->addMenu(tr("&Working Copy"));
    modifyMenu = menuBar()->addMenu(tr("&Modify"));
    queryMenu = menuBar()->addMenu(tr("&Query"));
    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

}

void QSvn::createToolBar()
{
}


//protected slots
void QSvn::exitSlot()
{
/*todo:

    Config::Exemplar()->saveChanges();
    Config::Exemplar()->saveMainWindow( this );
    FileList::releaseExemplar();
    WorkingCopy::releaseExemplar();
*/
    qApp->exit( 0 );
}

/*todo:

void QSvn::addWorkingCopySlot()
{
    WorkingCopy::Exemplar()->addExistingWorkingCopySlot();
}

void QSvn::removeWorkingCopySlot()
{
    if ( QMessageBox::question( this, tr( "Confirmation" ), tr( "Would you really remove this Working Copy?" ),
                                QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        WorkingCopy::Exemplar()->removeCurrentWorkingCopySlot();
    }
}

void QSvn::checkoutSlot()
{
    WorkingCopy::Exemplar()->checkoutSlot();
}

void QSvn::svnCommand( int svnCommandType, bool withFileSelector )
{
    //already implemented: commit, revert
    if ( !WorkingCopy::Exemplar()->selectedWorkingCopyItem() )
        return;

    bool runCommand = FALSE;

    WorkingCopyItem *item = WorkingCopy::Exemplar()->selectedWorkingCopyItem();
    QStringList *fileList = new QStringList; // QStringList with filenames for svn command
    QString commitMessage;

    if ( withFileSelector )
        FileSelector::Exemplar()->initFileSelector( svnCommandType, item->fullPath() );

    //get the fileList depend on what widget has the focus
    if ( WorkingCopy::Exemplar()->getWidget()->hasFocus() )
    {
        //WorkingCopy has focus
        SvnWrapper::Exemplar()->filesToList( svnCommandType, fileList, item->fullPath() );
        if ( withFileSelector )
            FileSelector::Exemplar()->setSelectedFiles( fileList );
    }
    else if ( FileList::Exemplar()->getWidget()->hasFocus() )
    {
        //FileList has focus
        fileList = FileList::Exemplar()->selectedFileListItems();
        if ( withFileSelector )
            FileSelector::Exemplar()->setSelectedFiles( fileList );
    }
    else
    {
        QMessageBox::information( this, "QSvn", tr( "Please select a Working Copy or some Files for SVN Command" ) );
        return;
    }

    if ( withFileSelector )
    {
        if ( FileSelector::Exemplar()->exec() )
        {
            fileList = FileSelector::Exemplar()->selectedFiles();
            commitMessage = FileSelector::Exemplar()->messageString();
            runCommand = TRUE;
        }
    }
    else
        runCommand = TRUE;

    if ( runCommand )
    {
        //call svnCommand
        SvnWrapper::Exemplar()->doSvnCommand( svnCommandType, item->fullPath(), fileList, commitMessage );
        //Updates
        WorkingCopy::Exemplar()->updateElement( item );
        FileList::Exemplar()->updateListSlot( item->fullPath() );
    }
}


void QSvn::updateSlot()
{
    svnCommand( SvnWrapper::Update );
}

void QSvn::commitSlot()
{
    svnCommand( SvnWrapper::Commit, TRUE );
}

void QSvn::addSlot()
{
    svnCommand( SvnWrapper::Add, TRUE );
}

void QSvn::removeSlot()
{
    svnCommand( SvnWrapper::Remove, TRUE );
}

void QSvn::revertSlot()
{
    svnCommand( SvnWrapper::Revert, TRUE );
}

void QSvn::diffSlot()
{
    SvnWrapper::Exemplar()->diff( WorkingCopy::Exemplar()->selectedWorkingCopyItem()->fullPath(), FileList::Exemplar()->selectedFileListItems(), FALSE );
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
    AboutDlg about( this );
    about.exec();
}

void QSvn::aboutQtSlot()
{
    QMessageBox::aboutQt( this );
}
*/
