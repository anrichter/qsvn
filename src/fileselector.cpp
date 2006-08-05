/***************************************************************************
 *   This file is part of QSvn Project http://ar.oszine.de/projects/qsvn   *
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
#include "config.h"
#include "filelistitem.h"
#include "filelistmodel.h"
#include "fileselector.h"
#include "svnclient.h"

//Qt
#include <QtGui>


FileSelector::FileSelector( QWidget *parent, SvnClient::SvnAction svnAction,
                            QItemSelectionModel *selectionModel,
                            FileListModel::SelectionFrom selectionFrom )
        : QDialog( parent )
{
    setupUi( this );
    m_fileListModel = 0;
    m_svnAction = svnAction,
    m_selectionModel = selectionModel;
    m_selectionFrom = selectionFrom;

    createMenus();
    initModel();

    configUI();
    setupConnections();
    treeViewFiles->installEventFilter( this );

    Config::instance()->restoreWidget( this, this->windowTitle() );
    Config::instance()->restoreHeaderView( this, treeViewFiles->header() );
}

void FileSelector::initModel()
{
    if ( m_fileListModel )
        delete( m_fileListModel );

    m_fileListModel = new FileListModel( this, m_svnAction );
    switch( m_selectionFrom )
    {
        case FileListModel::File:
            //todo: m_fileListModel->loadFromFileListSelection( m_selectionModel );
            break;
        case FileListModel::WorkingCopy:
            m_fileListModel->loadFromWorkingCopySelection( m_selectionModel );
            break;
    }
    treeViewFiles->setModel( m_fileListModel );
}

void FileSelector::configUI()
{
    switch ( m_svnAction )
    {
        case SvnClient::SvnNone:
            setWindowTitle( "" );
            hideGroupBoxLogMessage();
            break;
        case SvnClient::SvnAdd:
            setWindowTitle( tr( "Add") );
            setWindowIcon( QIcon( ":actionaddlocal.png" ) );
            hideGroupBoxLogMessage();
            break;
        case SvnClient::SvnCommit:
            setWindowTitle( tr( "Commit") );
            setWindowIcon( QIcon( ":actioncommit.png" ) );
            comboLogHistory->addItems( Config::instance()->getStringList( "logHistory" ) );
            comboLogHistory->insertItem( 0, "" );
            comboLogHistory->setCurrentIndex( 0 );
            Config::instance()->restoreSplitter( this, splitter );
            break;
        case SvnClient::SvnDelete:
            setWindowTitle( tr( "Delete") );
            setWindowIcon( QIcon( ":actiondeletelocal.png" ) );
            hideGroupBoxLogMessage();
            break;
        case SvnClient::SvnRevert:
            setWindowTitle( tr( "Revert") );
            setWindowIcon( QIcon( ":actionrevert.png" ) );
            hideGroupBoxLogMessage();
            break;
    }
}

void FileSelector::createMenus()
{
    contextMenu = new QMenu( this );

    if ( ( m_svnAction == SvnClient::SvnCommit ) ||
         ( m_svnAction == SvnClient::SvnRevert ) )
    {
        contextMenu->addAction( actionDiff );
        contextMenu->addAction( actionRevert );
        actionRevert->setIcon( QIcon( ":actionrevert.png" ) );
    }
}

void FileSelector::setupConnections( )
{
    connect( treeViewFiles, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( diff( const QModelIndex & ) ) );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( buttonOkClickedSlot() ) );
    connect( comboLogHistory, SIGNAL( activated( int ) ), this, SLOT( comboLogHistoryActivatedSlot( int ) ) );
    connect( checkSelectAll, SIGNAL( stateChanged( int ) ), this, SLOT( checkSelectAllStateChanged( int ) ) );

    connect( actionDiff, SIGNAL( triggered() ), this, SLOT( doDiff() ) );
    connect( actionRevert, SIGNAL( triggered() ), this, SLOT( doRevert() ) );
}

int FileSelector::exec()
{
    checkSelectAll->setCheckState( Qt::CheckState( Config::instance()->value( "selectAll" + this->windowTitle() ).toInt() ) );
    return QDialog::exec();
}

FileSelector::~ FileSelector( )
{
    Config::instance()->saveWidget( this, this->windowTitle() );
    Config::instance()->setValue( "selectAll" + this->windowTitle(), checkSelectAll->checkState() );
}

FileListModel *FileSelector::model( )
{
    return m_fileListModel;
}

void FileSelector::hideGroupBoxLogMessage( )
{
    groupBoxLogMessage->setVisible( false );
}

QStringList FileSelector::selectedFileList( )
{
    QStringList fileList;
    QModelIndex index;
    for ( int i = 0; i < m_fileListModel->rowCount(); ++i )
    {
        index = m_fileListModel->index( i, FileListItem::FilenameColumn );
        if ( m_fileListModel->data( index, Qt::CheckStateRole ).toBool() )
        {
            fileList << m_fileListModel->data( index, FileListModel::FullFileNameRole ).toString();
        }
    }
    return fileList;
}

QString FileSelector::logMessage( )
{
    return editLogMessage->toPlainText();
}

void FileSelector::buttonOkClickedSlot()
{
    if ( m_svnAction == SvnClient::SvnCommit )
    {
        QStringList logEntries;
        logEntries << editLogMessage->toPlainText();
        for ( int i = 0; i < comboLogHistory->count(); ++i )
        {
            if ( !comboLogHistory->itemText( i ).isEmpty() && !logEntries.contains( comboLogHistory->itemText( i ) ) )
                logEntries << comboLogHistory->itemText( i );
        }
        Config::instance()->saveStringList( "logHistory", logEntries );
        Config::instance()->saveSplitter( this, splitter );
    }

    Config::instance()->saveHeaderView( this, treeViewFiles->header() );
    this->accept();
}

void FileSelector::comboLogHistoryActivatedSlot( int index )
{
    editLogMessage->setPlainText( comboLogHistory->itemText( index ) );
}

void FileSelector::checkSelectAllStateChanged( int state )
{
    QModelIndex index;
    for ( int i = 0; i < m_fileListModel->rowCount(); ++i )
    {
        index = m_fileListModel->index( i, FileListItem::FilenameColumn );
        m_fileListModel->setData( index, state, Qt::CheckStateRole );
    }
}

bool FileSelector::eventFilter( QObject * watched, QEvent * event )
{
    if ( watched == treeViewFiles )
    {
        if ( event->type() == QEvent::ContextMenu )
        {
            contextMenu->popup( static_cast< QContextMenuEvent* >( event )->globalPos() );
        }
    }
    return QDialog::eventFilter( watched, event );
}

void FileSelector::doRevert( )
{
    QString fullFileName;
    fullFileName = m_fileListModel->data( treeViewFiles->selectionModel()->currentIndex(),
                                          FileListModel::FullFileNameRole ).toString();

    if ( QMessageBox::question( this, tr( "Revert" ),
                                QString( tr( "Do you really want to revert local changes from\n%1?" ) ).arg( fullFileName ),
                                QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        SvnClient::instance()->revert( fullFileName, false );

        m_fileListModel->removeRows( static_cast< FileListItem* >( treeViewFiles->selectionModel()->currentIndex().internalPointer() )->row(), 1 );
        treeViewFiles->dataChanged( m_fileListModel->index( 0, 0 ),
                                    m_fileListModel->index( m_fileListModel->rowCount(), m_fileListModel->columnCount() ) );
    }
}

void FileSelector::diff( const QModelIndex &index )
{
    SvnClient::instance()->diff( m_fileListModel->data( index, FileListModel::FullFileNameRole ).toString() );
}

void FileSelector::doDiff( )
{
    diff( treeViewFiles->selectionModel()->currentIndex() );
}

#include "fileselector.moc"
