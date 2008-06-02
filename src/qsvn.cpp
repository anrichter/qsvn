/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2008 Andreas Richter <ar@anrichter.net>                 *
 *                                                                              *
 *   This program is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License Version 2             *
 *   as published by the Free Software Foundation.                              *
 *                                                                              *
 *   This program is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 *   GNU General Public License for more details.                               *
 *                                                                              *
 *   You should have received a copy of the GNU General Public License          *
 *   along with this program; if not, write to the                              *
 *   Free Software Foundation, Inc.,                                            *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                  *
 *                                                                              *
 *******************************************************************************/

//QSvn
#include "checkout.h"
#include "config.h"
#include "configure.h"
#include "filelistproxy.h"
#include "filemodifier.h"
#include "fileselector.h"
#include "helper.h"
#include "merge.h"
#include "pathproperties.h"
#include "qsvn_defines.h"
#include "qsvn.h"
#include "showlog.h"
#include "statusentriesmodel.h"
#include "statustext.h"
#include "svnclient.h"
#include "wcmodel.h"

//SvnCpp
#include "svnqt/status.hpp"
#include "svnqt/version_check.hpp"

//Qt
#include <QtGui>


QSvn::QSvn(QWidget *parent, Qt::WFlags flags)
        : QMainWindow(parent, flags)
{
    setupUi(this);
    setWindowIcon(QIcon(":/images/qsvn.png"));

    //make sure, that the svn-client is correct initialised.
    SvnClient::instance();

    createMenus();

    StatusText::setOut(editStatusText);

    //setup wcModel
    wcModel = new WcModel(this);
    treeViewWorkingCopy->setModel(wcModel);
    treeViewWorkingCopy->setSortingEnabled(true);
    treeViewWorkingCopy->sortByColumn(0, Qt::AscendingOrder);
    treeViewWorkingCopy->installEventFilter(this);
    treeViewWorkingCopy->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //setup fileListModel
    m_statusEntriesModel = new StatusEntriesModel(this);
    m_fileListProxy = new FileListProxy(this);
    m_fileListProxy->setSourceModel(m_statusEntriesModel);
    treeViewFileList->setModel(m_fileListProxy);
    treeViewFileList->setSortingEnabled(true);
    treeViewFileList->sortByColumn(0, Qt::AscendingOrder);
    treeViewFileList->installEventFilter(this);
    treeViewFileList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(treeViewWorkingCopy->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));

    connect(treeViewFileList, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(on_actionDiff_triggered()));

    connect(treeViewWorkingCopy, SIGNAL(collapsed(const QModelIndex &)),
            wcModel, SLOT(doCollapse(const QModelIndex &)));

    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

    Config::instance()->restoreMainWindow(this);
    Config::instance()->restoreHeaderView(this, treeViewFileList->header());
}

void QSvn::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.indexes().count() == 1) //only change the FileList-View if one WC is selected
    {
        QModelIndex index = selected.indexes().at(0);
        m_currentWCpath = wcModel->getPath(index);
        m_statusEntriesModel->readDirectory(m_currentWCpath, false, false);
    }
}

QSvn::~QSvn()
{
    Config::instance()->saveMainWindow(this);
    Config::instance()->saveHeaderView(this, treeViewFileList->header());
    Config::instance()->removeTempDir();
}

void QSvn::createMenus()
{
    contextMenuWorkingCopy = new QMenu(this);
    contextMenuWorkingCopy->addAction(actionUpdate);
    contextMenuWorkingCopy->addAction(actionCommit);
    contextMenuWorkingCopy->addSeparator();
    contextMenuWorkingCopy->addAction(actionShowLog);
    contextMenuWorkingCopy->addAction(actionMerge);
    contextMenuWorkingCopy->addSeparator();
    contextMenuWorkingCopy->addAction(actionRevert);
    contextMenuWorkingCopy->addAction(actionWcCleanup);
    contextMenuWorkingCopy->addSeparator();
    contextMenuWorkingCopy->addAction(actionAdd);
    contextMenuWorkingCopy->addAction(actionDelete);
    contextMenuWorkingCopy->addAction(actionMkDir);
    contextMenuWorkingCopy->addAction(actionEditProperties);
    contextMenuWorkingCopy->addSeparator();
    contextMenuWorkingCopy->addAction(actionWcRemoveFromFavorites);
    contextMenuWorkingCopy->addAction(actionWcRemoveFromDisk);

    contextMenuFileList = new QMenu(this);
    contextMenuFileList->addAction(actionUpdate);
    contextMenuFileList->addAction(actionCommit);
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction(actionDiff);
    contextMenuFileList->addAction(actionShowLog);
    contextMenuFileList->addAction(actionMerge);
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction(actionRevert);
    contextMenuFileList->addAction(actionFlResolved);
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction(actionAdd);
    contextMenuFileList->addAction(actionDelete);
    contextMenuFileList->addAction(actionFlRename);
    contextMenuFileList->addAction(actionFlMove);
    contextMenuFileList->addAction(actionFlCopy);
    contextMenuFileList->addAction(actionEditProperties);
    contextMenuFileList->addSeparator();
    contextMenuFileList->addAction(actionFlRemoveFromDisk);
}

bool QSvn::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == treeViewWorkingCopy)
    {
        if (event->type() == QEvent::ContextMenu)
        {
            contextMenuWorkingCopy->popup(static_cast<QContextMenuEvent*>(event)->globalPos());
        }
    }
    else if (watched == treeViewFileList)
    {
        if (event->type() == QEvent::ContextMenu)
        {
            contextMenuFileList->popup(static_cast<QContextMenuEvent*>(event)->globalPos());
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void QSvn::closeEvent(QCloseEvent *event)
{
    qApp->exit();
}

bool QSvn::isFileListSelected()
{
    if (treeViewFileList->hasFocus() &&
            (treeViewFileList->selectionModel()->selection().count() > 0))
        return true;
    else
        return false;
}

QStringList QSvn::selectedPaths()
{
    QSet<QString> pathSet;

    if (isFileListSelected())
    {
        //todo: extract to method selectedFiles
        QModelIndexList indexes = treeViewFileList->selectionModel()->selectedIndexes();
        svn::StatusPtr status;

        for (int i = 0; i < indexes.count(); ++i)
        {
            status = m_statusEntriesModel->at(m_fileListProxy->mapToSource(indexes.at(i)).row());
            pathSet << QDir::toNativeSeparators(status->path());
        }
    }
    else
    {
        //todo: extract to method selectedWorkingCopies
        QModelIndexList indexes = treeViewWorkingCopy->selectionModel()->selectedIndexes();

        for (int i = 0; i < indexes.count(); i++)
        {
            pathSet << wcModel->getPath(indexes.at(i));
        }
    }

    return pathSet.toList();
}

void QSvn::setActionStop(QString aText)
{
    if (aText.isEmpty())
    {
        actionStop->setText("");
        disconnect(actionStop, SIGNAL(triggered()), 0, 0);
    }
    else
    {
        actionStop->setText("Stop " + aText);
        connect(actionStop, SIGNAL(triggered()),
                SvnClient::instance(), SLOT(setCancel()));
    }

    actionStop->setEnabled(!aText.isEmpty());

    qApp->processEvents();
}

void QSvn::directoryChanged(const QString &dir)
{
    if (m_currentWCpath == dir)
        m_statusEntriesModel->readDirectory(m_currentWCpath, false, true);
}

//private slots
void QSvn::on_actionWcAdd_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                  tr("Select a working Directory"),
                  Config::instance()->value(KEY_LASTWC).toString(),
                  QFileDialog::ShowDirsOnly);

    if (!dir.isEmpty())
        wcModel->insertWc(dir);
}

void QSvn::on_actionWcRemoveFromFavorites_triggered()
{
    foreach(QString path, selectedPaths())
    {
        if (QMessageBox::question(this,
            tr("Confirmation"),
               tr("Are you sure you want to remove the directory \n   %1 \nfrom the list of favorites?").arg(path),
                  QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            wcModel->removeWc(path);
        }
    }
}

void QSvn::on_actionWcRemoveFromDisk_triggered()
{
    foreach(QString path, selectedPaths())
    {
        if (QMessageBox::question(this,
            tr("Confirmation"),
               tr("Are you sure you want to delete the directory \n   %1 \npermanently from the disk?").arg(path),
                  QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            wcModel->removeWc(path);
            Helper::removeFromDisk(path);
        }
    }
}

void QSvn::on_actionWcCheckout_triggered()
{
    Checkout checkout(this);

    if (checkout.exec())
    {
        setActionStop("Checkout");

        if (SvnClient::instance()->checkout(checkout.url(), checkout.path()))
        {
            wcModel->insertWc(checkout.path());
        }

        setActionStop("");
    }
}

void QSvn::on_actionUpdate_triggered()
{
    setActionStop("Update");
    SvnClient::instance()->update(selectedPaths(), isFileListSelected());
    setActionStop("");

    m_statusEntriesModel->readDirectory(m_currentWCpath, false, true);
}

void QSvn::on_actionCommit_triggered()
{
    FileSelector::doSvnAction(this, SvnClient::SvnCommit, selectedPaths(), isFileListSelected());
}

void QSvn::on_actionAdd_triggered()
{
    FileSelector::doSvnAction(this, SvnClient::SvnAdd, selectedPaths(), isFileListSelected());
}

void QSvn::on_actionDelete_triggered()
{
    FileSelector::doSvnAction(this, SvnClient::SvnDelete, selectedPaths(), isFileListSelected());
}

void QSvn::on_actionRevert_triggered()
{
    FileSelector::doSvnAction(this, SvnClient::SvnRevert, selectedPaths(), isFileListSelected());
}

void QSvn::on_actionShowLog_triggered()
{
    foreach(QString path, selectedPaths())
    {
        ShowLog::doShowLog(0, path, svn::Revision::HEAD, svn::Revision::START);
    }
}

void QSvn::on_actionWcCleanup_triggered()
{
    if (!isFileListSelected())
    {
        QStringList cleanupList = selectedPaths();

        setActionStop("Cleanup");

        for (int i = 0; i < cleanupList.count(); i++)
        {
            SvnClient::instance()->cleanup(cleanupList.at(i));
        }

        setActionStop("");
    }
}

void QSvn::on_actionDiff_triggered()
{
    if (isFileListSelected())
        SvnClient::instance()->diffBASEvsWORKING(selectedPaths());
}

void QSvn::on_actionConfigureQSvn_triggered()
{
    Configure configure(this);
    configure.exec();
}

void QSvn::on_actionAboutQSvn_triggered()
{
    QString aboutMsg = "";
    aboutMsg += "<div align=\"center\">";
    aboutMsg += "<h2>QSvn</h2>";
    aboutMsg += QString(QSVN_VERSION_STRING);

    aboutMsg += "<p>is a graphical Subversion Client.<br/>";
    aboutMsg += "(c) 2004-2008 Andreas Richter<br/>";
    aboutMsg += "<a href=\"http://www.anrichter.net/projects/qsvn/\">http://www.anrichter.net/projects/qsvn/</a></p>";

    aboutMsg += "<p>It runs with";
    aboutMsg += "<br/>Subversion " + svn::Version::linked_version();
    aboutMsg += "<br/>Qt " + QString(QT_VERSION_STR);
    aboutMsg += "</p>";

    aboutMsg += "<p>This Program is released under the terms of the<br/>GNU GENERAL PUBLIC LICENSE Version 2, June 1991</p>";
    aboutMsg += "</div>";
    QMessageBox::about(this, "Caption", aboutMsg);
}

void QSvn::on_actionFlResolved_triggered()
{
    if (isFileListSelected())
    {
        QStringList resolveList = selectedPaths();

        setActionStop("Resolve");

        for (int i = 0; i < resolveList.count(); i++)
        {
            if (QMessageBox::question(this, tr("Confirmation"),
                                      QString(tr("Are you sure that\n%1\nis resolved?")
                                              .arg(resolveList.at(i))),
                                      QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)

                SvnClient::instance()->resolved(resolveList.at(i));
        }

        setActionStop("Resolved finished");

        m_statusEntriesModel->readDirectory(m_currentWCpath, false, true);
    }
}

void QSvn::on_actionFlRename_triggered()
{
    if (isFileListSelected())
    {
        foreach(QString path, selectedPaths())
        {
            FileModifier fm(this, path, SvnClient::SvnRename);
            fm.exec();
        }
    }
}

void QSvn::on_actionFlMove_triggered()
{
    if (isFileListSelected())
    {
        foreach(QString path, selectedPaths())
        {
            FileModifier fm(this, path, SvnClient::SvnMove);
            fm.exec();
        }
    }
}

void QSvn::on_actionFlCopy_triggered()
{
    if (isFileListSelected())
    {
        foreach(QString path, selectedPaths())
        {
            FileModifier fm(this, path, SvnClient::SvnCopy);
            fm.exec();
        }
    }
}

void QSvn::on_actionMkDir_triggered()
{
    FileModifier fm(this, m_currentWCpath, SvnClient::SvnMkDir);
    fm.exec();
}

void QSvn::on_actionEditProperties_triggered()
{
    foreach(QString path, selectedPaths())
    {
        PathProperties::doPathProperties(this, path);
    }
}

void QSvn::on_actionFlRemoveFromDisk_triggered()
{
    if (isFileListSelected())
        FileSelector::doSvnAction(this, SvnClient::RemoveFromDisk, selectedPaths(), true);
}

void QSvn::on_actionMerge_triggered()
{
    foreach(QString path, selectedPaths())
    {
        Merge::doMerge(path);
    }
}

#include "qsvn.moc"
