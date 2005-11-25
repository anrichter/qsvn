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
            hideGroupBoxLogMessage();
            break;
        case FileListModel::Commit:
            setWindowTitle( tr( "Commit") );
            break;
        case FileListModel::Remove:
            setWindowTitle( tr( "Remove") );
            hideGroupBoxLogMessage();
            break;
        case FileListModel::Revert:
            setWindowTitle( tr( "Revert") );
            hideGroupBoxLogMessage();
            break;
    }

    connect( treeViewFiles, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( diff( const QModelIndex & ) ) );
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
