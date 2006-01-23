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
#include "config.h"
#include "filelistitem.h"
#include "filelistmodel.h"
#include "fileselector.h"
#include "svnclient.h"

#include <QtGui>


FileSelector::FileSelector( QWidget *parent, FileListModel::ModelFor modelFor )
        : QDialog( parent )
{
    setupUi( this );
    m_fileListModel = new FileListModel( this, modelFor );
    treeViewFiles->setModel( m_fileListModel );

    switch ( modelFor )
    {
    case FileListModel::Add:
        setWindowTitle( tr( "Add") );
        setWindowIcon( QIcon( ":menuadd.png" ) );
        hideGroupBoxLogMessage();
        break;
    case FileListModel::Commit:
        setWindowTitle( tr( "Commit") );
        setWindowIcon( QIcon( ":menucommit.png" ) );
        break;
    case FileListModel::Delete:
        setWindowTitle( tr( "Delete") );
        setWindowIcon( QIcon( ":menudelete.png" ) );
        hideGroupBoxLogMessage();
        break;
    case FileListModel::Revert:
        setWindowTitle( tr( "Revert") );
        setWindowIcon( QIcon( ":menurevert.png" ) );
        hideGroupBoxLogMessage();
        break;
    }

    Config::instance()->restoreWidget( this, this->windowTitle() );

    comboLogHistory->addItems( Config::instance()->getStringList( "logHistory" ) );
    comboLogHistory->insertItem( 0, "" );
    comboLogHistory->setCurrentIndex( 0 );

    connect( treeViewFiles, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( diff( const QModelIndex & ) ) );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( buttonOkClickedSlot() ) );
    connect( comboLogHistory, SIGNAL( activated( int ) ), this, SLOT( comboLogHistoryActivatedSlot( int ) ) );
    connect( checkSelectAll, SIGNAL( stateChanged( int ) ), this, SLOT( checkSelectAllStateChanged( int ) ) );
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

void FileSelector::diff( const QModelIndex &index )
{
    SvnClient::instance()->diff( m_fileListModel->data( index, FileListModel::FullFileNameRole ).toString() );
}

void FileSelector::buttonOkClickedSlot()
{
    QStringList logEntries;
    logEntries << editLogMessage->toPlainText();
    for ( int i = 0; i < comboLogHistory->count(); ++i )
    {
        if ( !comboLogHistory->itemText( i ).isEmpty() && !logEntries.contains( comboLogHistory->itemText( i ) ) )
            logEntries << comboLogHistory->itemText( i );
    }
    Config::instance()->saveStringList( "logHistory", logEntries );
    this->accept();
}

void FileSelector::comboLogHistoryActivatedSlot( int index )
{
    editLogMessage->setPlainText( comboLogHistory->currentText() );
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
