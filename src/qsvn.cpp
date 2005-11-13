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
 ***************************************************************************/


//QSvn
#include "addworkingcopy.h"
#include "checkout.h"
#include "config.h"
#include "configure.h"
#include "filelistmodel.h"
#include "filelistitem.h"
#include "qsvn.h"
#include "statustext.h"
#include "svnclient.h"
#include "workingcopymodel.h"
#include "workingcopyitem.h"

/*todo:
#include "AboutDlg.h"
#include "FileSelector.h"
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

    StatusText::instance()->setOutPutWidget( editStatusText );

    //setup workingCopyModel
    workingCopyModel = new WorkingCopyModel();
    treeViewWorkingCopy->setModel( workingCopyModel );

    //setup fileListModel
    fileListModel = new FileListModel();
    treeViewFileList->setModel( fileListModel );
    treeViewFileList->header()->setSortIndicatorShown( true );
    treeViewFileList->header()->setClickable( true );

    connect( treeViewWorkingCopy, SIGNAL( clicked( const QModelIndex & ) ), this, SLOT( activateWorkingCopy( const QModelIndex & ) ) );

    Config::instance()->restoreMainWindow( this );
}

void QSvn::activateWorkingCopy( const QModelIndex &index )
{
    if ( index.isValid() )
        fileListModel->setActiveDirectory( workingCopyModel->data( index, WorkingCopyModel::FullDirectory ).toString() );
    else
        fileListModel->setActiveDirectory( "" );
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
    connect( actionRemoveWorkingCopy, SIGNAL( triggered() ), this, SLOT( removeWorkingCopySlot() ) );
    actionCheckout = new QAction( "&Checkout...", this );
    connect( actionCheckout, SIGNAL( triggered() ), this, SLOT( checkoutSlot() ) );

    actionUpdate = new QAction( "&Update", this );
    connect( actionUpdate, SIGNAL( triggered() ), this, SLOT( updateSlot() ) );
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
    Config::instance()->saveMainWindow( this );
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

void QSvn::removeWorkingCopySlot()
{
    if ( QMessageBox::question( this, tr( "Confirmation" ), tr( "Would you really remove this Working Copy?" ),
                                QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {

        QItemSelectionModel *selectionModel = treeViewWorkingCopy->selectionModel();
        QModelIndexList indexes = selectionModel->selectedIndexes();

        for ( int i = 0; i < indexes.count(); i++ )
        {
            workingCopyModel->removeRow( indexes.at( i ).row() );
        }
        activateWorkingCopy( QModelIndex() );
    }
}

void QSvn::checkoutSlot()
{
    Checkout checkout( this );
    if ( checkout.exec() )
    {
        if ( SvnClient::instance()->checkout( checkout.url(), checkout.path() ) )
        {
            workingCopyModel->addWorkingCopy( checkout.path() );
        }
    }
}

void QSvn::updateSlot()
{
    QStringList updateList;

    if ( treeViewWorkingCopy->hasFocus() )
    {
        QItemSelectionModel *selectionModel = treeViewWorkingCopy->selectionModel();
        QModelIndexList indexes = selectionModel->selectedIndexes();

        if ( indexes.count() == 0 )
        {
            QMessageBox::information( this, "QSvn", "You must select a Working Copy", QMessageBox::Ok );
            return;
        }

        for ( int i = 0; i < indexes.count(); i++ )
        {
            updateList << static_cast< WorkingCopyItem* >( indexes.at( i ).internalPointer() )->fullPath();
        }
    } else if ( treeViewFileList->hasFocus() )
    {
        QItemSelectionModel *selectionModel = treeViewFileList->selectionModel();
        QModelIndexList indexes = selectionModel->selectedIndexes();

        if ( indexes.count() == 0 )
        {
            QMessageBox::information( this, "QSvn", "You must select a File", QMessageBox::Ok );
            return;
        }

        for ( int i = 0; i < indexes.count(); i++ )
        {
            updateList << static_cast< FileListItem* >( indexes.at( i ).internalPointer() )->fullFileName();
        }
    } else {
        QMessageBox::information( this, "QSvn", "You must select a Working Copy", QMessageBox::Ok );
    }

    if ( updateList.count() > 0 )
        SvnClient::instance()->update( updateList );
}

/*
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
    StatusText::instance()->outputMessage( QString( "Help Content - not implemented yet" ) );
}

void QSvn::helpIndexSlot()
{
    //todo: implement
    StatusText::instance()->outputMessage( QString( "Help Index - not implemented yet" ) );
}

/* todo:
void QSvn::aboutSlot()
{
    AboutDlg about( this );
    about.exec();
}
*/
