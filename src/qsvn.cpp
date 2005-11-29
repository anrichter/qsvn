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
#include "about.h"
#include "addworkingcopy.h"
#include "checkout.h"
#include "config.h"
#include "configure.h"
#include "filelistmodel.h"
#include "filelistitem.h"
#include "fileselector.h"
#include "qsvn.h"
#include "statustext.h"
#include "svnclient.h"
#include "workingcopymodel.h"
#include "workingcopyitem.h"

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
    connect( treeViewFileList, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( diff() ) );

    Config::instance()->restoreMainWindow( this );
}

void QSvn::activateWorkingCopy( const QModelIndex &index )
{
    if ( index.isValid() )
    {
        fileListModel->removeAllRows();
        fileListModel->loadFromDirectory( workingCopyModel->data( index, WorkingCopyModel::FullDirectory ).toString() );
    }
    else
    {
        fileListModel->removeAllRows();
    }
}

QSvn::~QSvn()
{
    delete( workingCopyModel );
}

void QSvn::createActions()
{
    actionExit = new QAction( "E&xit", this );
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( exit() ) );

    actionAddWorkingCopy = new QAction( "&Add...", this );
    connect( actionAddWorkingCopy, SIGNAL( triggered() ), this, SLOT( doAddWorkingCopy() ) );
    actionRemoveWorkingCopy = new QAction( "&Remove...", this );
    connect( actionRemoveWorkingCopy, SIGNAL( triggered() ), this, SLOT( doRemoveWorkingCopy() ) );
    actionCheckoutWorkingCopy = new QAction( "&Checkout...", this );
    connect( actionCheckoutWorkingCopy, SIGNAL( triggered() ), this, SLOT( doCheckoutWorkingCopy() ) );

    actionUpdate = new QAction( "&Update", this );
    connect( actionUpdate, SIGNAL( triggered() ), this, SLOT( doUpdate() ) );
    actionCommit = new QAction( "&Commit...", this );
    connect( actionCommit, SIGNAL( triggered() ), this, SLOT( doCommit() ) );
    actionAdd = new QAction( "&Add...", this );
    connect( actionAdd, SIGNAL( triggered() ), this, SLOT( doAdd() ) );
    actionDelete = new QAction( "&Delete...", this );
    connect( actionDelete, SIGNAL( triggered() ), this, SLOT( doDelete() ) );
    actionRevert = new QAction( "Re&vert...", this );
    connect( actionRevert, SIGNAL( triggered() ), this, SLOT( doRevert() ) );

    actionDiff = new QAction( "&Diff...", this );
    connect( actionDiff, SIGNAL( triggered() ), this, SLOT( doDiff() ) );

    actionConfigureQSvn = new QAction( "&Configure QSvn...", this );
    connect( actionConfigureQSvn, SIGNAL( triggered() ), this, SLOT( configureQSvn() ) );

    actionHelpContents = new QAction( "&Contents...", this );
    connect( actionHelpContents, SIGNAL( triggered() ), this, SLOT( helpContents() ) );
    actionHelpIndex = new QAction( "&Index...", this );
    connect( actionHelpIndex, SIGNAL( triggered() ), this, SLOT( helpIndex() ) );
    actionAboutQSvn = new QAction( "&About QSvn...", this );
    connect( actionAboutQSvn, SIGNAL( triggered() ), this, SLOT( aboutQSvn() ) );
    actionAboutQt = new QAction( "About &Qt...", this );
    connect( actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
}

void QSvn::createMenus()
{
    menuFile = menuBar()->addMenu( tr( "&File" ) );
    menuFile->addAction( actionExit );

    menuWorkingCopy = menuBar()->addMenu( tr( "&Working Copy" ) );
    menuWorkingCopy->addAction( actionAddWorkingCopy );
    menuWorkingCopy->addAction( actionRemoveWorkingCopy );
    menuWorkingCopy->addAction( actionCheckoutWorkingCopy );

    menuModify = menuBar()->addMenu( tr( "&Modify" ) );
    menuModify->addAction( actionUpdate );
    menuModify->addAction( actionCommit );
    menuModify->addSeparator();
    menuModify->addAction( actionAdd );
    menuModify->addAction( actionDelete );
    menuModify->addAction( actionRevert );

    menuQuery = menuBar()->addMenu( tr( "&Query" ) );
    menuQuery->addAction( actionDiff );

    menuSettings = menuBar()->addMenu( tr( "&Settings" ) );
    menuSettings->addAction( actionConfigureQSvn );

    menuHelp = menuBar()->addMenu( tr( "&Help" ) );
    menuHelp->addAction( actionHelpContents );
    menuHelp->addAction( actionHelpIndex );
    menuHelp->addSeparator();
    menuHelp->addAction( actionAboutQSvn );
    menuHelp->addAction( actionAboutQt );
}

void QSvn::createToolBar()
{}

bool QSvn::isFileListSelected()
{
    if ( treeViewFileList->hasFocus() && ( treeViewFileList->selectionModel()->selection().count() > 0 ) )
        return true;
    else
        return false;
}

QStringList QSvn::selectedFiles()
{
    QSet<QString> fileSet;
    QModelIndexList indexes = treeViewFileList->selectionModel()->selectedIndexes();

    for ( int i = 0; i < indexes.count(); ++i )
    {
        fileSet << static_cast< FileListItem* >( indexes.at( i ).internalPointer() )->fullFileName();
    }

    return fileSet.toList();
}

//protected slots
void QSvn::exit()
{
    Config::instance()->saveMainWindow( this );
    qApp->exit( 0 );
}

void QSvn::doAddWorkingCopy()
{
    AddWorkingCopy addWorkingCopy( this );
    if ( addWorkingCopy.exec() )
    {
        workingCopyModel->addWorkingCopy( addWorkingCopy.getSelectedDirectory() );
    }
}

void QSvn::doRemoveWorkingCopy()
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

void QSvn::doCheckoutWorkingCopy()
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

void QSvn::doUpdate()
{
    QSet<QString> updateSet;

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
            updateSet << static_cast< WorkingCopyItem* >( indexes.at( i ).internalPointer() )->fullPath();
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
            updateSet << static_cast< FileListItem* >( indexes.at( i ).internalPointer() )->fullFileName();
        }
    } else {
        QMessageBox::information( this, "QSvn", "You must select a Working Copy", QMessageBox::Ok );
    }

    if ( updateSet.count() > 0 )
    {
        QStringList updateList = updateSet.toList();
        SvnClient::instance()->update( updateList );
    }
}

void QSvn::doCommit()
{
    FileSelector fileselector( this, FileListModel::Commit );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
        SvnClient::instance()->commit( fileselector.selectedFileList(), fileselector.logMessage() );
}

void QSvn::doAdd()
{
    FileSelector fileselector( this, FileListModel::Add );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
        SvnClient::instance()->add( fileselector.selectedFileList() );
}

void QSvn::doDelete()
{
    FileSelector fileselector( this, FileListModel::Delete );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
        SvnClient::instance()->remove( fileselector.selectedFileList() );
}

void QSvn::doRevert()
{
    FileSelector fileselector( this, FileListModel::Revert );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
        SvnClient::instance()->revert( fileselector.selectedFileList() );
}

void QSvn::doDiff()
{
    SvnClient::instance()->diff( selectedFiles() );
}

void QSvn::configureQSvn()
{
    Configure configure( this );
    configure.exec();
}

void QSvn::helpContents()
{
    //todo: implement
    StatusText::instance()->outputMessage( QString( "not implemented yet..." ) );
}

void QSvn::helpIndex()
{
    //todo: implement
    StatusText::instance()->outputMessage( QString( "not implemented yet" ) );
}

void QSvn::aboutQSvn()
{
    About about( this );
    about.exec();
}
