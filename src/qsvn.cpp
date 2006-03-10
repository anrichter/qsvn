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

//SvnCpp
#include "svnqt/version_check.hpp"

//Qt
#include <QtGui>


QSvn::QSvn( QWidget *parent, Qt::WFlags flags )
        : QMainWindow( parent, flags )
{
    setupUi( this );
    setWindowIcon( QIcon( ":qsvn.png" ) );

    setActionIcons();
    connectActions();
    createMenus();

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

void QSvn::setActionIcons( )
{
    //todo: Qt4.1.0 Designer can't handle icons set in ui file :(
    actionAddWorkingCopy->setIcon( QIcon( ":menuadd.png" ) );
    actionRemoveWorkingCopy->setIcon( QIcon( ":menudelete.png" ) );
    actionCheckoutWorkingCopy->setIcon( QIcon( ":menucheckout.png" ) );

    actionUpdate->setIcon( QIcon( ":menuupdate.png" ) );
    actionCommit->setIcon( QIcon( ":menucommit.png" ) );
    actionAdd->setIcon( QIcon( ":menuadd.png" ) );
    actionDelete->setIcon( QIcon( ":menudelete.png" ) );
    actionRevert->setIcon( QIcon( ":menurevert.png" ) );

    actionStop->setIcon( QIcon( ":actionstop.png" ) );
}

void QSvn::connectActions()
{
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );

    connect( actionAddWorkingCopy, SIGNAL( triggered() ), this, SLOT( doAddWorkingCopy() ) );
    connect( actionRemoveWorkingCopy, SIGNAL( triggered() ), this, SLOT( doRemoveWorkingCopy() ) );
    connect( actionCheckoutWorkingCopy, SIGNAL( triggered() ), this, SLOT( doCheckoutWorkingCopy() ) );

    connect( actionUpdate, SIGNAL( triggered() ), this, SLOT( doUpdate() ) );
    connect( actionCommit, SIGNAL( triggered() ), this, SLOT( doCommit() ) );
    connect( actionAdd, SIGNAL( triggered() ), this, SLOT( doAdd() ) );
    connect( actionDelete, SIGNAL( triggered() ), this, SLOT( doDelete() ) );
    connect( actionRevert, SIGNAL( triggered() ), this, SLOT( doRevert() ) );

    connect( actionDiff, SIGNAL( triggered() ), this, SLOT( doDiff() ) );

    connect( actionConfigureQSvn, SIGNAL( triggered() ), this, SLOT( configureQSvn() ) );

    connect( actionAboutQSvn, SIGNAL( triggered() ), this, SLOT( aboutQSvn() ) );
    connect( actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

    connect( actionStop, SIGNAL( triggered( ) ), SvnClient::instance(), SLOT( setCancel( ) ) );
}

void QSvn::createMenus()
{
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
    AddWorkingCopy::addWorkingCopy( this, workingCopyModel );
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
        setActionStop( "Checkout" );
        if ( SvnClient::instance()->checkout( checkout.url(), checkout.path() ) )
        {
            workingCopyModel->addWorkingCopy( checkout.path() );
        }
        setActionStop( "" );
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
        setActionStop( "Update" );
        SvnClient::instance()->update( updateList );
        setActionStop( "" );
    }
    activateWorkingCopy( treeViewWorkingCopy->selectionModel()->currentIndex() );
}

void QSvn::doCommit()
{
    FileSelector fileselector( this, FileListModel::Commit );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
    {
        setActionStop( "Commit" );
        SvnClient::instance()->commit( fileselector.selectedFileList(), fileselector.logMessage() );
        setActionStop( "" );
    }
    activateWorkingCopy( treeViewWorkingCopy->selectionModel()->currentIndex() );
}

void QSvn::doAdd()
{
    FileSelector fileselector( this, FileListModel::Add );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
    {
        setActionStop( "Add" );
        SvnClient::instance()->add( fileselector.selectedFileList() );
        setActionStop( "" );
    }
    activateWorkingCopy( treeViewWorkingCopy->selectionModel()->currentIndex() );
}

void QSvn::doDelete()
{
    FileSelector fileselector( this, FileListModel::Delete );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
    {
        setActionStop( "Delete" );
        SvnClient::instance()->remove( fileselector.selectedFileList() );
        setActionStop( "" );
    }
    activateWorkingCopy( treeViewWorkingCopy->selectionModel()->currentIndex() );
}

void QSvn::doRevert()
{
    FileSelector fileselector( this, FileListModel::Revert );
    if ( isFileListSelected() )
        fileselector.model()->loadFromFileListSelection( treeViewFileList->selectionModel() );
    else
        fileselector.model()->loadFromWorkingCopySelection( treeViewWorkingCopy->selectionModel() );

    if ( fileselector.exec() )
    {
        setActionStop( "Revert" );
        SvnClient::instance()->revert( fileselector.selectedFileList() );
        setActionStop( "" );
    }
    activateWorkingCopy( treeViewWorkingCopy->selectionModel()->currentIndex() );
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
    aboutMsg += "<br/>0.4.0 pre";
    aboutMsg += "<br/>Build with Subversion " + svn::Version::linked_version();
    aboutMsg += "<br/>Running with Subversion " + svn::Version::running_version();
    aboutMsg += "<p>QSvn is a graphical Subversion Client.<br/>";
    aboutMsg += "(c) 2004-2006 Andreas Richter<br/>";
    aboutMsg += "<a href=\"http://ar.oszine.de/projects/qsvn/\">http://ar.oszine.de/projects/qsvn/</a></p>";
    aboutMsg += "<p>This Program is released under the terms of the<br/>GNU GENERAL PUBLIC LICENSE Version 2, June 1991</p>";
    aboutMsg += "</div>";
    QMessageBox::about( this, "Caption", aboutMsg );
}

void QSvn::setActionStop( QString aText )
{
    if ( aText.isEmpty() )
    {
        actionStop->setText( "" );
        disconnect( actionStop, SIGNAL( triggered( ) ), 0, 0 );
    }
    else
    {
        actionStop->setText( "Stop " + aText );
        connect( actionStop, SIGNAL( triggered( ) ), SvnClient::instance(), SLOT( setCancel( ) ) );
    }
    actionStop->setEnabled( !aText.isEmpty() );
    qApp->processEvents();
}
