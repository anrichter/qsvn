/***************************************************************************
 *   This file is part of QSvn Project http://ar.oszine.de/projects/qsvn   *
 *   Copyright (c) 2004-2007 Andreas Richter <ar@oszine.de>                *
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
#include "checkout.h"
#include "config.h"
#include "configure.h"
#include "filelistproxy.h"
#include "filemodifier.h"
#include "fileselector.h"
#include "qsvn_defines.h"
#include "qsvn.h"
#include "showlog.h"
#include "statustext.h"
#include "svnclient.h"
#include "wcmodel.h"

//SvnCpp
#include "svnqt/version_check.hpp"

//Qt
#include <QtGui>


QSvn::QSvn( QWidget *parent, Qt::WFlags flags )
        : QMainWindow( parent, flags )
{
    setupUi( this );
    setWindowIcon( QIcon( ":/images/qsvn.png" ) );

    connectActions();
    createMenus();

    StatusText::setOut( editStatusText );

    //setup wcModel
    wcModel = new WcModel( this );
    treeViewWorkingCopy->setModel( wcModel );
/*    treeViewWorkingCopy->header()->setSortIndicatorShown( true );
    treeViewWorkingCopy->header()->setClickable( true );*/
    treeViewWorkingCopy->installEventFilter( this );

    //setup fileListModel
    fileListProxy = new FileListProxy( this );
    treeViewFileList->setModel( fileListProxy );
    treeViewFileList->header()->setSortIndicatorShown( true );
    treeViewFileList->header()->setClickable( true );
    treeViewFileList->installEventFilter( this );

    connect( treeViewWorkingCopy, SIGNAL( clicked( const QModelIndex & ) ), this, SLOT( activateWorkingCopy( const QModelIndex & ) ) );
    connect( treeViewFileList, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( doDiff() ) );

    connect( treeViewWorkingCopy, SIGNAL( collapsed( const QModelIndex & ) ), wcModel, SLOT( doUpdate( const QModelIndex & ) ) );
    connect( treeViewWorkingCopy, SIGNAL( expanded( const QModelIndex & ) ), wcModel, SLOT( doUpdate( const QModelIndex & ) ) );

    Config::instance()->restoreMainWindow( this );
    Config::instance()->restoreHeaderView( this, treeViewFileList->header() );
}

void QSvn::activateWorkingCopy( const QModelIndex &index, const bool force )
{
    if ( index.isValid() )
    {
        m_currentWCpath = wcModel->getPath( index );
        fileListProxy->readDirectory( m_currentWCpath, false, force );
    }
}

QSvn::~QSvn()
{
    Config::instance()->saveMainWindow( this );
    Config::instance()->saveHeaderView( this, treeViewFileList->header() );
    Config::instance()->removeTempDir();
    delete( fileListProxy );
    delete( contextMenuWorkingCopy );
    delete( contextMenuFileList );
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
    connect( actionLog, SIGNAL( triggered() ), this, SLOT( doShowLog() ) );
    connect( actionCleanup, SIGNAL( triggered() ), this, SLOT( doCleanup() ) );
    connect( actionResolved, SIGNAL( triggered() ), this, SLOT( doResolved() ) );
    connect( actionRename, SIGNAL( triggered() ), this, SLOT( doRename() ) );
    connect( actionMove, SIGNAL( triggered() ), this, SLOT( doMove() ) );
    connect( actionCopy, SIGNAL( triggered() ), this, SLOT( doCopy() ) );

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
    contextMenuWorkingCopy->addAction( actionLog );
    contextMenuWorkingCopy->addSeparator();
    contextMenuWorkingCopy->addAction( actionAdd );
    contextMenuWorkingCopy->addAction( actionDelete );
    contextMenuWorkingCopy->addAction( actionRevert );
    contextMenuWorkingCopy->addAction( actionCleanup );
    contextMenuWorkingCopy->addSeparator();
    contextMenuWorkingCopy->addAction( actionAddWorkingCopy );
    contextMenuWorkingCopy->addAction( actionRemoveWorkingCopy );
    contextMenuWorkingCopy->addAction( actionCheckoutWorkingCopy );

    contextMenuFileList = new QMenu( this );
    contextMenuFileList->addAction( actionDiff );
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction( actionUpdate );
    contextMenuFileList->addAction( actionCommit );
    contextMenuFileList->addAction( actionLog );
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction( actionAdd );
    contextMenuFileList->addAction( actionDelete );
    contextMenuFileList->addAction( actionRevert );
    contextMenuFileList->addAction( actionResolved );
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction( actionRename );
    contextMenuFileList->addAction( actionMove );
    contextMenuFileList->addAction( actionCopy );
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

void QSvn::closeEvent( QCloseEvent * event )
{
    qApp->exit();
}

bool QSvn::isFileListSelected()
{
    if ( treeViewFileList->hasFocus() && ( treeViewFileList->selectionModel()->selection().count() > 0 ) )
        return true;
    else
        return false;
}

QStringList QSvn::selectedPaths()
{
    QSet<QString> pathSet;

    if ( isFileListSelected() )
    {
        QModelIndexList indexes = treeViewFileList->selectionModel()->selectedIndexes();
        svn::Status status;

        for ( int i = 0; i < indexes.count(); ++i )
        {
            status = fileListProxy->at( indexes.at( i ) );
            pathSet << QDir::toNativeSeparators( status.path() );
        }
    }
    else
    {
        QModelIndexList indexes = treeViewWorkingCopy->selectionModel()->selectedIndexes();
        for ( int i = 0; i < indexes.count(); i++ )
        {
            pathSet << wcModel->getPath( indexes.at( i ) );
        }
    }
    return pathSet.toList();
}

//private slots
void QSvn::doAddWorkingCopy()
{
    QString dir = QFileDialog::getExistingDirectory( this,
            tr( "Select a working Directory" ), "", QFileDialog::ShowDirsOnly );
    if ( !dir.isEmpty() )
        wcModel->addWc( dir );
}

void QSvn::doRemoveWorkingCopy()
{
    QItemSelectionModel *selectionModel = treeViewWorkingCopy->selectionModel();
    QModelIndexList indexes = selectionModel->selectedIndexes();

    for ( int i = 0; i < indexes.count(); i++ )
    {
        if ( QMessageBox::question( this, tr( "Confirmation" ), tr( "Should i really remove this Working Copy?" ),
                                    QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
        {
            wcModel->removeWc( indexes.at( i ) );
        }
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
            wcModel->addWc( checkout.path() );
        }
        setActionStop( "" );
    }
}

void QSvn::doUpdate()
{
    setActionStop( "Update" );
    SvnClient::instance()->update( selectedPaths(), isFileListSelected() );
    setActionStop( "" );

    activateWorkingCopy( treeViewWorkingCopy->selectionModel()->currentIndex(), true );
}

void QSvn::doCommit()
{
    FileSelector::doSvnAction( SvnClient::SvnCommit, selectedPaths(), isFileListSelected() );
}

void QSvn::doAdd()
{
    FileSelector::doSvnAction( SvnClient::SvnAdd, selectedPaths(), isFileListSelected() );
}

void QSvn::doDelete()
{
    FileSelector::doSvnAction( SvnClient::SvnDelete, selectedPaths(), isFileListSelected() );
}

void QSvn::doRevert()
{
    FileSelector::doSvnAction( SvnClient::SvnRevert, selectedPaths(), isFileListSelected() );
}

void QSvn::doShowLog()
{
    foreach( QString path, selectedPaths() )
        ShowLog::doShowLog( 0, path, svn::Revision::HEAD, svn::Revision::START );
}

void QSvn::doCleanup()
{
    if ( !isFileListSelected() )
    {
        QStringList cleanupList = selectedPaths();

        setActionStop( "Cleanup" );
        for ( int i = 0; i < cleanupList.count(); i++ )
        {
            SvnClient::instance()->cleanup( cleanupList.at( i ) );
        }
        setActionStop( "" );
    }
}

void QSvn::doDiff()
{
    if ( isFileListSelected() )
        SvnClient::instance()->diff( selectedPaths() );
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
    aboutMsg += QString( "<br/> %1.%2.%3" )
                .arg( QSVN_MAJOR_VERSION )
                .arg( QSVN_MINOR_VERSION )
                .arg( QSVN_MICRO_VERSION );
    aboutMsg += "<br/>Build with Subversion " + svn::Version::linked_version();
    aboutMsg += "<br/>Running with Subversion " + svn::Version::running_version();
    aboutMsg += "<p>QSvn is a graphical Subversion Client.<br/>";
    aboutMsg += "(c) 2004-2007 Andreas Richter<br/>";
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

void QSvn::doResolved( )
{
    if ( isFileListSelected() )
    {
        QStringList resolveList = selectedPaths();

        setActionStop( "Resolve" );
        for ( int i = 0; i < resolveList.count(); i++ )
        {
            if ( QMessageBox::question( this, tr( "Confirmation" ), QString( tr( "Are you sure that\n%1\nis resolved?" ).arg( resolveList.at( i ) ) ),
                                        QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )

                SvnClient::instance()->resolved( resolveList.at( i ) );
        }
        setActionStop( "Cleanup finished" );
        activateWorkingCopy( treeViewWorkingCopy->selectionModel()->currentIndex(), true );
    }
}

void QSvn::doRename()
{
    if ( isFileListSelected() )
    {
        foreach( QString path, selectedPaths() )
        {
            FileModifier fm( this, path, SvnClient::SvnRename );
            fm.exec();
        }
    }
}

void QSvn::doMove()
{
    if ( isFileListSelected() )
    {
        foreach( QString path, selectedPaths() )
        {
            FileModifier fm( this, path, SvnClient::SvnMove );
            fm.exec();
        }
    }
}

void QSvn::doCopy()
{
    if ( isFileListSelected() )
    {
        foreach( QString path, selectedPaths() )
        {
            FileModifier fm( this, path, SvnClient::SvnCopy );
            fm.exec();
        }
    }
}

#include "qsvn.moc"
