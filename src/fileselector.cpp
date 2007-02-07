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
#include "config.h"
#include "fileselector.h"
#include "fileselectorproxy.h"
#include "svnclient.h"

//Qt
#include <QtGui>

FileSelector::FileSelector( const SvnClient::SvnAction svnAction,
                            const QStringList pathList, const bool isFileList )
        : QDialog( 0 )
{
    setupUi( this );

    m_fileSelectorProxy = new FileSelectorProxy( this, svnAction );
    if ( isFileList )
        m_fileSelectorProxy->readFileList( pathList );
    else
        m_fileSelectorProxy->readDirectory( pathList.at( 0 ), true );
    setupFileSelector( svnAction );
    checkSelectAll->setCheckState( Qt::CheckState( Config::instance()->value( "selectAll" + this->windowTitle() ).toInt() ) );
}

FileSelector::~FileSelector( )
{
    Config::instance()->saveWidget( this, this->windowTitle() );
    Config::instance()->setValue( "selectAll" + this->windowTitle(), checkSelectAll->checkState() );
    if ( m_fileSelectorProxy )
        delete( m_fileSelectorProxy );
}

void FileSelector::setupFileSelector( SvnClient::SvnAction svnAction )
{
    m_svnAction = svnAction;

    treeViewFiles->setModel( m_fileSelectorProxy );

    setupMenus();
    setupUI();
    setupConnections();

    treeViewFiles->installEventFilter( this );

    Config::instance()->restoreWidget( this, this->windowTitle() );
    Config::instance()->restoreHeaderView( this, treeViewFiles->header() );
}

void FileSelector::setupUI()
{
    switch ( m_svnAction )
    {
    case SvnClient::SvnNone:
        setWindowTitle( "" );
        groupBoxLogMessage->setVisible( false );
        break;
    case SvnClient::SvnAdd:
        setWindowTitle( tr( "Add") );
        setWindowIcon( QIcon( ":/images/actionaddlocal.png" ) );
        groupBoxLogMessage->setVisible( false );
        break;
    case SvnClient::SvnCommit:
        setWindowTitle( tr( "Commit") );
        setWindowIcon( QIcon( ":/images/actioncommit.png" ) );
        comboLogHistory->addItems( Config::instance()->getStringList( "logHistory" ) );
        comboLogHistory->insertItem( 0, "" );
        comboLogHistory->setCurrentIndex( 0 );
        Config::instance()->restoreSplitter( this, splitter );
        break;
    case SvnClient::SvnDelete:
        setWindowTitle( tr( "Delete") );
        setWindowIcon( QIcon( ":/images/actiondeletelocal.png" ) );
        groupBoxLogMessage->setVisible( false );
        break;
    case SvnClient::SvnRevert:
        setWindowTitle( tr( "Revert") );
        setWindowIcon( QIcon( ":/images/actionrevert.png" ) );
        groupBoxLogMessage->setVisible( false );
        break;
    }
}

void FileSelector::setupMenus()
{
    contextMenu = new QMenu( this );

    if ( ( m_svnAction == SvnClient::SvnCommit ) ||
            ( m_svnAction == SvnClient::SvnRevert ) )
    {
        contextMenu->addAction( actionDiff );
        contextMenu->addAction( actionRevert );
        contextMenu->addAction( actionResolved );
        actionRevert->setIcon( QIcon( ":/images/actionrevert.png" ) );
    }
}

void FileSelector::setupConnections( )
{
    connect( okButton, SIGNAL( clicked() ), this, SLOT( buttonOkClickedSlot() ) );
    connect( comboLogHistory, SIGNAL( activated( int ) ), this, SLOT( comboLogHistoryActivatedSlot( int ) ) );
    connect( checkSelectAll, SIGNAL( stateChanged( int ) ), this, SLOT( checkSelectAllStateChanged( int ) ) );
    connect( treeViewFiles->selectionModel(),
             SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
             this, SLOT( updateActions( const QItemSelection &, const QItemSelection & ) ) );

    connect( actionDiff, SIGNAL( triggered() ), this, SLOT( doDiff() ) );
    connect( actionRevert, SIGNAL( triggered() ), this, SLOT( doRevert() ) );
    connect( actionResolved, SIGNAL( triggered() ), this, SLOT( doResolved() ) );

    connect( treeViewFiles, SIGNAL( doubleClicked( const QModelIndex & ) ),
        this, SLOT( doDiff() ) );

}

void FileSelector::showModeless()
{
    setAttribute( Qt::WA_DeleteOnClose, true );
    show();
    raise();
    activateWindow();
}

void FileSelector::buttonOkClickedSlot()
{
    if ( m_svnAction == SvnClient::SvnCommit )
    {
        if ( ( editLogMessage->toPlainText().isEmpty() ) &&
              ( Config::instance()->value( KEY_CHECKEMPTYLOGMESSAGE ).toBool() ) )
        {
            if ( QMessageBox::question( this, tr( "Commit without Log Message" ),
                                        tr( "Would you really commit your Changes without a Log Message?"),
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
            {
                return;
            }
        }

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

    //call svn actions
    setEnabled( false );
    qApp->processEvents();
    switch ( m_svnAction )
    {
        case SvnClient::SvnAdd:
            SvnClient::instance()->add( m_fileSelectorProxy->checkedFileList() );
            break;
        case SvnClient::SvnCommit:
            SvnClient::instance()->commit( m_fileSelectorProxy->checkedFileList(), editLogMessage->toPlainText() );
            break;
        case SvnClient::SvnDelete:
            SvnClient::instance()->remove( m_fileSelectorProxy->checkedFileList() );
            break;
        case SvnClient::SvnRevert:
            SvnClient::instance()->revert( m_fileSelectorProxy->checkedFileList(), true );
    }
    setEnabled( true );
    qApp->processEvents();
    this->accept();
}

void FileSelector::comboLogHistoryActivatedSlot( int index )
{
    editLogMessage->setPlainText( comboLogHistory->itemText( index ) );
}

void FileSelector::checkSelectAllStateChanged( int state )
{
    m_fileSelectorProxy->setSelectAllState( state );
}

bool FileSelector::eventFilter( QObject * watched, QEvent * event )
{
    if ( watched == treeViewFiles )
    {
        if ( event->type() == QEvent::ContextMenu )
            contextMenu->popup( static_cast< QContextMenuEvent* >( event )->globalPos() );
    }
    return QDialog::eventFilter( watched, event );
}

void FileSelector::doRevert( )
{
    QModelIndex index = treeViewFiles->selectionModel()->currentIndex();

    QString fullFileName;
    fullFileName = m_fileSelectorProxy->at( index ).path();

    if ( QMessageBox::question( this, tr( "Revert" ),
                                QString( tr( "Do you really want to revert local changes from\n%1?" ) ).arg( fullFileName ),
                                QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        SvnClient::instance()->revert( fullFileName, false );

        m_fileSelectorProxy->updateEntry( index );
    }
}

void FileSelector::doResolved()
{
    QModelIndex index = treeViewFiles->selectionModel()->currentIndex();

    QString fullFileName;
    fullFileName = m_fileSelectorProxy->at( index ).path();

    if ( QMessageBox::question( this, tr( "Resolved" ),
                                QString( tr( "Do you really want to mark %1 as resolved\n?" ) ).arg( fullFileName ),
                                QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        SvnClient::instance()->resolved( fullFileName );

        m_fileSelectorProxy->updateEntry( index, false );
    }
}


void FileSelector::doDiff()
{
    if ( !actionDiff->isEnabled() )
        return;

    //todo: multiselect in treeViewFiles and call SvnClient::instance()->diff wiht a QStringList
    SvnClient::instance()->diff( m_fileSelectorProxy->at( treeViewFiles->selectionModel()->currentIndex() ).path() );
}

void FileSelector::updateActions( const QItemSelection &selected, const QItemSelection &deselected )
{
    svn::Status _status = m_fileSelectorProxy->at( selected.indexes().at( 0 ) );

    actionDiff->setEnabled( ( _status.textStatus() == svn_wc_status_modified ) ||
        ( _status.textStatus() == svn_wc_status_conflicted ) );
    actionResolved->setEnabled( _status.textStatus() == svn_wc_status_conflicted );
}

//static functions
void FileSelector::doSvnAction( const SvnClient::SvnAction svnAction,
                                const QStringList pathList, const bool isFileList )
{
    FileSelector *fs = new FileSelector( svnAction, pathList, isFileList );
    fs->showModeless();
}

#include "fileselector.moc"
