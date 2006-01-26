/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2006 Andreas Richter <ar@oszine.de>                *
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
    setWindowIcon( QIcon( ":qsvn.png" ) );

    createActions();
    createMenus();
    createToolBar();

    StatusText::instance()->setOutPutWidget( editStatusText );

    //setup workingCopyModel
    workingCopyModel = new WorkingCopyModel();
    treeViewWorkingCopy->setModel( workingCopyModel );
    treeViewWorkingCopy->header()->setSortIndicatorShown( true );
    treeViewWorkingCopy->header()->setClickable( true );
    treeViewWorkingCopy->installEventFilter( this );

    //setup fileListModel
    fileListModel = new FileListModel();
    treeViewFileList->setModel( fileListModel );
    treeViewFileList->header()->setSortIndicatorShown( true );
    treeViewFileList->header()->setClickable( true );
    treeViewFileList->installEventFilter( this );

    connect( treeViewWorkingCopy, SIGNAL( clicked( const QModelIndex & ) ), this, SLOT( activateWorkingCopy( const QModelIndex & ) ) );
    connect( treeViewFileList, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( doDiff() ) );

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
    Config::instance()->saveMainWindow( this );
    delete( workingCopyModel );
    delete( contextMenuWorkingCopy );
    delete( contextMenuFileList );
}

void QSvn::createActions()
{
    actionExit = new QAction( tr( "E&xit" ), this );
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );

    actionAddWorkingCopy = new QAction( QIcon( ":menuadd.png" ), tr( "&Add..." ), this );
    connect( actionAddWorkingCopy, SIGNAL( triggered() ), this, SLOT( doAddWorkingCopy() ) );
    actionRemoveWorkingCopy = new QAction( QIcon( ":menudelete.png" ), tr( "&Remove..." ), this );
    connect( actionRemoveWorkingCopy, SIGNAL( triggered() ), this, SLOT( doRemoveWorkingCopy() ) );
    actionCheckoutWorkingCopy = new QAction( QIcon( ":menucheckout.png" ), tr( "&Checkout..." ), this );
    connect( actionCheckoutWorkingCopy, SIGNAL( triggered() ), this, SLOT( doCheckoutWorkingCopy() ) );

    actionUpdate = new QAction( QIcon( ":menuupdate.png" ), tr( "&Update" ), this );
    connect( actionUpdate, SIGNAL( triggered() ), this, SLOT( doUpdate() ) );
    actionCommit = new QAction( QIcon( ":menucommit.png" ), tr( "&Commit..." ), this );
    connect( actionCommit, SIGNAL( triggered() ), this, SLOT( doCommit() ) );
    actionAdd = new QAction( QIcon( ":menuadd.png" ), tr( "&Add..." ), this );
    connect( actionAdd, SIGNAL( triggered() ), this, SLOT( doAdd() ) );
    actionDelete = new QAction( QIcon( ":menudelete.png" ), tr( "&Delete..." ), this );
    connect( actionDelete, SIGNAL( triggered() ), this, SLOT( doDelete() ) );
    actionRevert = new QAction( QIcon( ":menurevert.png" ), tr( "Re&vert..." ), this );
    connect( actionRevert, SIGNAL( triggered() ), this, SLOT( doRevert() ) );

    actionDiff = new QAction( tr( "&Diff..." ), this );
    connect( actionDiff, SIGNAL( triggered() ), this, SLOT( doDiff() ) );

    actionConfigureQSvn = new QAction( tr( "&Configure QSvn..." ), this );
    connect( actionConfigureQSvn, SIGNAL( triggered() ), this, SLOT( configureQSvn() ) );

    actionAboutQSvn = new QAction( tr( "&About QSvn..." ), this );
    connect( actionAboutQSvn, SIGNAL( triggered() ), this, SLOT( aboutQSvn() ) );
    actionAboutQt = new QAction( tr( "About &Qt..." ), this );
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
    menuHelp->addAction( actionAboutQSvn );
    menuHelp->addAction( actionAboutQt );

    contextMenuWorkingCopy = new QMenu( this );
    contextMenuWorkingCopy->addAction( actionUpdate );
    contextMenuWorkingCopy->addAction( actionCommit );
    contextMenuWorkingCopy->addSeparator();
    contextMenuWorkingCopy->addAction( actionAdd );
    contextMenuWorkingCopy->addAction( actionDelete );
    contextMenuWorkingCopy->addAction( actionRevert );
    contextMenuWorkingCopy->addSeparator();
    contextMenuWorkingCopy->addAction( actionAddWorkingCopy );
    contextMenuWorkingCopy->addAction( actionRemoveWorkingCopy );
    contextMenuWorkingCopy->addAction( actionCheckoutWorkingCopy );

    contextMenuFileList = new QMenu( this );
    contextMenuFileList->addAction( actionDiff );
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction( actionUpdate );
    contextMenuFileList->addAction( actionCommit );
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction( actionAdd );
    contextMenuFileList->addAction( actionDelete );
    contextMenuFileList->addAction( actionRevert );
}

void QSvn::createToolBar()
{}

bool QSvn::eventFilter( QObject * watched, QEvent * event )
{
    if ( watched == treeViewWorkingCopy )
    {
        if ( event->type() == QEvent::ContextMenu )
        {
            contextMenuWorkingCopy->popup( static_cast< QContextMenuEvent* >( event )->globalPos() );
        }
    }
    else if ( watched == treeViewFileList )
    {
        if ( event->type() == QEvent::ContextMenu )
        {
            contextMenuFileList->popup( static_cast< QContextMenuEvent* >( event )->globalPos() );
        }
    }
    return QMainWindow::eventFilter( watched, event );
}

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
    QItemSelectionModel *selectionModel = treeViewWorkingCopy->selectionModel();
    QModelIndexList indexes = selectionModel->selectedIndexes();

    for ( int i = 0; i < indexes.count(); i++ )
    {
        if ( QMessageBox::question( this, tr( "Confirmation" ), tr( "Would you really remove this Working Copy?" ),
                                    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            workingCopyModel->removeRow( indexes.at( i ).row() );
    }
    activateWorkingCopy( QModelIndex() );
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
            QMessageBox::information( this, "QSvn", tr( "You must select a Working Copy" ), QMessageBox::Ok );
            return;
        }

        for ( int i = 0; i < indexes.count(); i++ )
        {
            updateSet << static_cast< WorkingCopyItem* >( indexes.at( i ).internalPointer() )->fullPath();
        }
    }
    else if ( treeViewFileList->hasFocus() )
    {
        QItemSelectionModel *selectionModel = treeViewFileList->selectionModel();
        QModelIndexList indexes = selectionModel->selectedIndexes();

        if ( indexes.count() == 0 )
        {
            QMessageBox::information( this, "QSvn", tr( "You must select a File" ), QMessageBox::Ok );
            return;
        }

        for ( int i = 0; i < indexes.count(); i++ )
        {
            updateSet << static_cast< FileListItem* >( indexes.at( i ).internalPointer() )->fullFileName();
        }
    }
    else
    {
        QMessageBox::information( this, "QSvn", tr( "You must select a Working Copy" ), QMessageBox::Ok );
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
        SvnClient::instance()->add
        ( fileselector.selectedFileList() );
}

void QSvn::doDelete()
{
    FileSelector fileselector( this, FileListModel::Delete );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
        SvnClient::instance()->remove
        ( fileselector.selectedFileList() );
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

void QSvn::aboutQSvn()
{
    QString aboutMsg = "";
    aboutMsg += "<div align=\"center\">";
    aboutMsg += "<h2>QSvn</h2>";
    aboutMsg += "<br/>0.3.0-pre";
    aboutMsg += "<p>QSvn is a graphical Subversion Client.<br/>";
    aboutMsg += "(c) 2004-2006 QSvn Team<br/>";
    aboutMsg += "<a href=\"http://qsvn.berlios.de\">http://qsvn.berlios.de</p>";
    aboutMsg += "<p>This Program is released under the terms of the<br/>GNU GENERAL PUBLIC LICENSE Version 2, June 1991</p>";
    aboutMsg += "</div>";
    QMessageBox::about( this, "Caption", aboutMsg );
}
