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
#include "addworkingcopy.h"
#include "config.h"
#include "configure.h"
#include "filelistmodel.h"
#include "qsvn.h"
#include "statustext.h"
#include "workingcopymodel.h"

/*todo:
#include "AboutDlg.h"
#include "FileSelector.h"
*/

//Qt
#include <QtGui>
#include <QModelIndex>


QSvn::QSvn( QWidget *parent, Qt::WFlags flags )
        : QMainWindow( parent, flags )
{
    setupUi( this );

    createActions();
    createMenus();
    createToolBar();

    StatusText::Exemplar()->setOutPutWidget( editStatusText );

    //setup workingCopyModel
    workingCopyModel = new WorkingCopyModel();
    treeViewWorkingCopy->setModel( workingCopyModel );

    //setup fileListModel
    fileListModel = new FileListModel();
    treeViewFileList->setModel( fileListModel );


    connect( treeViewWorkingCopy, SIGNAL( clicked( const QModelIndex & ) ), this, SLOT( activateWorkingCopy( const QModelIndex & ) ) );

    Config::Exemplar()->restoreMainWindow( this );
}

void QSvn::activateWorkingCopy( const QModelIndex &index )
{
    fileListModel->setActiveDirectory( workingCopyModel->data( index, WorkingCopyModel::FullDirectory ).toString() );
}

QSvn::~QSvn()
{
    delete( workingCopyModel );
}

void QSvn::createActions()
{
    actionExit = new QAction( "E&xit", this );
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( exitSlot() ) );

    actionAddWorkingCopy = new QAction( "&Add...", this );
    connect( actionAddWorkingCopy, SIGNAL( triggered() ), this, SLOT( addWorkingCopySlot() ) );
    actionRemoveWorkingCopy = new QAction( "&Remove...", this );
    actionCheckout = new QAction( "&Checkout...", this );

    actionUpdate = new QAction( "&Update", this );
    actionCommit = new QAction( "&Commit...", this );
    actionAdd = new QAction( "&Add...", this );
    actionRemove = new QAction( "&Remove...", this );
    actionRevert = new QAction( "Re&vert...", this );

    actionDiff = new QAction( "&Diff...", this );

    actionConfigureQSvn = new QAction( "&Configure QSvn...", this );
    connect( actionConfigureQSvn, SIGNAL( triggered() ), this, SLOT( configureQSvnSlot() ) );

    actionHelpContents = new QAction( "&Contents...", this );
    connect( actionHelpContents, SIGNAL( triggered() ), this, SLOT( helpContentsSlot() ) );
    actionHelpIndex = new QAction( "&Index...", this );
    connect( actionHelpIndex, SIGNAL( triggered() ), this, SLOT( helpIndexSlot() ) );
    actionAboutQSvn = new QAction( "&About QSvn...", this );
    actionAboutQt = new QAction( "About &Qt...", this );
    connect( actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
}

void QSvn::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File" ) );
    fileMenu->addAction( actionExit );

    workingCopyMenu = menuBar()->addMenu( tr( "&Working Copy" ) );
    workingCopyMenu->addAction( actionAddWorkingCopy );
    workingCopyMenu->addAction( actionRemoveWorkingCopy );
    workingCopyMenu->addAction( actionCheckout );

    modifyMenu = menuBar()->addMenu( tr( "&Modify" ) );
    modifyMenu->addAction( actionUpdate );
    modifyMenu->addAction( actionCommit );
    modifyMenu->addSeparator();
    modifyMenu->addAction( actionAdd );
    modifyMenu->addAction( actionRemove );
    modifyMenu->addAction( actionRevert );

    queryMenu = menuBar()->addMenu( tr( "&Query" ) );
    queryMenu->addAction( actionDiff );

    settingsMenu = menuBar()->addMenu( tr( "&Settings" ) );
    settingsMenu->addAction( actionConfigureQSvn );

    helpMenu = menuBar()->addMenu( tr( "&Help" ) );
    helpMenu->addAction( actionHelpContents );
    helpMenu->addAction( actionHelpIndex );
    helpMenu->addSeparator();
    helpMenu->addAction( actionAboutQSvn );
    helpMenu->addAction( actionAboutQt );
}

void QSvn::createToolBar()
{}

//protected slots
void QSvn::exitSlot()
{
    Config::Exemplar()->saveMainWindow( this );
    qApp->exit( 0 );
}

void QSvn::addWorkingCopySlot()
{
    AddWorkingCopy addWorkingCopy( this );
    if ( addWorkingCopy.exec() )
    {
        workingCopyModel->addWorkingCopy( addWorkingCopy.getSelectedDirectory() );
    }
}

/*todo:

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

*/

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

/* todo:
void QSvn::aboutSlot()
{
    AboutDlg about( this );
    about.exec();
}
*/
