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
#include "config.h"
#include "fileselector.h"
#include "fileselectorproxy.h"
#include "qsvn.h"
#include "showlog.h"
#include "statusentriesmodel.h"
#include "svnclient.h"

//SvnCpp
#include "svnqt/status.hpp"

//Qt
#include <QtGui>


FileSelector::FileSelector(QSvn *parent,
                           const SvnClient::SvnAction svnAction,
                           const QStringList pathList, const bool isFileList,
                           const QString wc)
        : QDialog(0) //don't set parent here! FileSelector is always a top-level window
{
    setupUi(this);

    qsvn = parent;
    m_wc = wc;
    m_cfgStrLogMessages = QString("logHistory_%1").arg(SvnClient::instance()->getUUID(m_wc));
    m_statusEntriesModel = new StatusEntriesModel(this);
    m_fileSelectorProxy = new FileSelectorProxy(this, svnAction);
    m_fileSelectorProxy->setSourceModel(m_statusEntriesModel);
    if (isFileList)
        m_statusEntriesModel->readFileList(pathList);
    else
        m_statusEntriesModel->readDirectory(pathList.at(0), svn::DepthInfinity, true);
    setupFileSelector(svnAction);
    checkSelectAll->setCheckState(Qt::CheckState(Config::instance()->value("selectAll" + SvnClient::instance()->getSvnActionName(m_svnAction)).toInt()));

    m_inClose = false;
}

FileSelector::~FileSelector()
{
    Config::instance()->saveWidget(this, SvnClient::instance()->getSvnActionName(m_svnAction));
    Config::instance()->setValue("selectAll" + SvnClient::instance()->getSvnActionName(m_svnAction), checkSelectAll->checkState());
}

void FileSelector::setupFileSelector(SvnClient::SvnAction svnAction)
{
    m_svnAction = svnAction;

    treeViewFiles->setModel(m_fileSelectorProxy);

    setupMenus();
    setupDlg();
    setupConnections();

    treeViewFiles->installEventFilter(this);

    Config::instance()->restoreWidget(this, SvnClient::instance()->getSvnActionName(m_svnAction));
    Config::instance()->restoreHeaderView(this, treeViewFiles->header());
}

void FileSelector::setupDlg()
{
    setWindowTitle(tr("%1 [%2]").arg(SvnClient::instance()->getSvnActionName(m_svnAction)).arg(m_wc));
    switch (m_svnAction)
    {
        case SvnClient::SvnNone:
            groupBoxLogMessage->setVisible(false);
            treeViewFiles->setFocus(Qt::MouseFocusReason);
            break;
        case SvnClient::SvnAdd:
            setWindowIcon(QIcon(":/images/actionaddlocal.svg"));
            groupBoxLogMessage->setVisible(false);
            treeViewFiles->setFocus(Qt::MouseFocusReason);
            break;
        case SvnClient::SvnCommit:
            setWindowIcon(QIcon(":/images/actioncommit.svg"));
            comboLogHistory->addItems(Config::instance()->getStringList(m_cfgStrLogMessages));
            comboLogHistory->insertItem(0, "");
            comboLogHistory->setCurrentIndex(0);
            Config::instance()->restoreSplitter(this, splitter);
            editLogMessage->setFocus(Qt::MouseFocusReason);
            break;
        case SvnClient::SvnDelete:
            setWindowIcon(QIcon(":/images/actiondeletelocal.svg"));
            groupBoxLogMessage->setVisible(false);
            treeViewFiles->setFocus(Qt::MouseFocusReason);
            break;
        case SvnClient::SvnRevert:
            setWindowIcon(QIcon(":/images/actionrevert.svg"));
            groupBoxLogMessage->setVisible(false);
            treeViewFiles->setFocus(Qt::MouseFocusReason);
            break;
        case SvnClient::RemoveFromDisk:
            setWindowIcon(QIcon(":/images/actiondeletelocal.svg"));
            groupBoxLogMessage->setVisible(false);
            treeViewFiles->setFocus(Qt::MouseFocusReason);
            break;
        default:
            groupBoxFiles->setVisible(false);
            groupBoxLogMessage->setVisible(false);
            break;
    }
}

void FileSelector::setupMenus()
{
    contextMenu = new QMenu(this);

    if ((m_svnAction == SvnClient::SvnCommit) ||
         (m_svnAction == SvnClient::SvnRevert))
    {
        contextMenu->addAction(actionDiff);
        contextMenu->addAction(actionRevert);
        contextMenu->addAction(actionResolved);
        actionRevert->setIcon(QIcon(":/images/actionrevert.svg"));
    }
    if ((m_svnAction == SvnClient::SvnCommit) ||
         (m_svnAction == SvnClient::SvnDelete) ||
         (m_svnAction == SvnClient::SvnRevert))
        contextMenu->addAction(actionShowLog);
}

void FileSelector::setupConnections()
{
    connect(treeViewFiles->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(updateActions(const QItemSelection &, const QItemSelection &)));
    connect(treeViewFiles,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
            SLOT(on_actionDiff_triggered()));
    connect(m_statusEntriesModel, SIGNAL(beginUpdate()),
        this, SLOT(onFsWatcherBeginUpdate()));
    connect(m_statusEntriesModel, SIGNAL(endUpdate()),
        this, SLOT(onFsWatcherEndUpdate()));
}

void FileSelector::showModeless()
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    show();
    raise();
    activateWindow();
}

void FileSelector::accept()
{
    m_inClose = true;
    m_statusEntriesModel->disableFsUpdates();
    if (m_svnAction == SvnClient::SvnCommit)
    {
        if ((editLogMessage->toPlainText().isEmpty()) &&
             (Config::instance()->value(KEY_CHECKEMPTYLOGMESSAGE).toBool()))
        {
            if (QMessageBox::question(this, tr("Commit without Log Message"),
                                      tr("Would you really commit your Changes without a Log Message?"),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
        }

        QStringList logEntries;
        logEntries << editLogMessage->toPlainText();
        for (int i = 0; i < comboLogHistory->count(); ++i)
        {
            if (!comboLogHistory->itemText(i).isEmpty() &&
                 !logEntries.contains(comboLogHistory->itemText(i)))
            {
                logEntries << comboLogHistory->itemText ( i );
            }
        }
        Config::instance()->saveStringList(m_cfgStrLogMessages, logEntries);
        Config::instance()->saveSplitter(this, splitter);
    }

    Config::instance()->saveHeaderView(this, treeViewFiles->header());

    //call svn actions
    setEnabled(false);
    qApp->processEvents();
    switch (m_svnAction)
    {
        case SvnClient::SvnAdd:
            SvnClient::instance()->add(m_fileSelectorProxy->checkedFileList(), svn::DepthEmpty);
            break;
        case SvnClient::SvnCommit:
            SvnClient::instance()->commit(m_fileSelectorProxy->checkedFileList(), editLogMessage->toPlainText());
            break;
        case SvnClient::SvnDelete:
            SvnClient::instance()->remove(m_fileSelectorProxy->checkedFileList());
            break;
        case SvnClient::SvnRevert:
            SvnClient::instance()->revert(m_fileSelectorProxy->checkedFileList(), true);
            break;
        case SvnClient::RemoveFromDisk:
            SvnClient::instance()->removeFromDisk(m_fileSelectorProxy->checkedFileList());
            break;
        default:
            break;
    }

    if (qsvn)
    {
        foreach(QString path, m_fileSelectorProxy->checkedFileList())
        {
            if (QFileInfo(path).isDir())
            {
                qsvn->updateWc(path);
            }
        }
    }
    QDialog::accept();
}

void FileSelector::reject()
{
    m_inClose = true;
    QDialog::reject();
}

void FileSelector::on_comboLogHistory_activated(int index)
{
    editLogMessage->setPlainText(comboLogHistory->itemText(index));
}

void FileSelector::on_checkSelectAll_stateChanged(int state)
{
    m_fileSelectorProxy->setSelectAllState(state);
}

bool FileSelector::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == treeViewFiles)
    {
        if (event->type() == QEvent::ContextMenu)
            contextMenu->popup(static_cast<QContextMenuEvent*>(event)->globalPos());
    }
    return QDialog::eventFilter(watched, event);
}

void FileSelector::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::ActivationChange && !m_inClose)
    {
        if (isActiveWindow())
        {
			m_statusEntriesModel->enableFsUpdates();
            m_statusEntriesModel->doFsUpdates();
        } else {
            m_statusEntriesModel->disableFsUpdates();
        }
    }
	QDialog::changeEvent(event);
}

void FileSelector::on_actionRevert_triggered()
{
    QModelIndex index = treeViewFiles->selectionModel()->currentIndex();

    QString fullFileName;
    fullFileName = m_statusEntriesModel->at(m_fileSelectorProxy->mapToSource(index).row())->path();

    if (QMessageBox::question(this, tr("Revert"),
                              QString(tr("Do you really want to revert local changes from\n%1?"))
                                      .arg(fullFileName),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        SvnClient::instance()->revert(fullFileName, false);
    }
}

void FileSelector::on_actionResolved_triggered()
{
    QModelIndex index = treeViewFiles->selectionModel()->currentIndex();

    QString fullFileName;
    fullFileName = m_statusEntriesModel->at(m_fileSelectorProxy->mapToSource(index).row())->path();

    if (QMessageBox::question(this, tr("Resolved"),
                              QString(tr("Do you really want to mark %1 as resolved\n?"))
                                      .arg(fullFileName),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        SvnClient::instance()->resolved(fullFileName);
    }
}


void FileSelector::on_actionShowLog_triggered()
{
    QModelIndex index = treeViewFiles->selectionModel()->currentIndex();

    QString fullFileName;
    fullFileName = m_statusEntriesModel->at(m_fileSelectorProxy->mapToSource(index).row())->path();

    ShowLog::doShowLog(0, fullFileName, svn::Revision::HEAD, svn::Revision::START);
}

void FileSelector::on_actionDiff_triggered()
{
    if (!actionDiff->isEnabled())
        return;

    //todo: multiselect in treeViewFiles and call SvnClient::instance()->diff wiht a QStringList
    SvnClient::instance()->diffBASEvsWORKING(m_statusEntriesModel->at(m_fileSelectorProxy->mapToSource(treeViewFiles->selectionModel()->currentIndex()).row())->path());
}

void FileSelector::updateActions(const QItemSelection &selected, const QItemSelection &deselected)
{
    svn::StatusPtr _status = m_statusEntriesModel->at(m_fileSelectorProxy->mapToSource(selected.indexes().at(0)).row());

    actionDiff->setEnabled((_status->textStatus() == svn_wc_status_modified) ||
                           (_status->textStatus() == svn_wc_status_conflicted));
    actionResolved->setEnabled(_status->textStatus() == svn_wc_status_conflicted);
}

void FileSelector::onFsWatcherBeginUpdate()
{
    setWindowTitle(tr("Update list [%1]").arg(m_wc));
    setEnabled(false);
    qApp->processEvents();
}

void FileSelector::onFsWatcherEndUpdate()
{
    setWindowTitle(tr("%1 [%2]").arg(SvnClient::instance()->getSvnActionName(m_svnAction)).arg(m_wc));
    setEnabled(true);
}

//static functions
void FileSelector::doSvnAction(QSvn *parent,
                               const SvnClient::SvnAction svnAction,
                               const QStringList pathList,
                               const bool isFileList,
                               const QString wc)
{
    if (isFileList)
    {
        FileSelector *fs = new FileSelector(parent, svnAction, pathList, isFileList, wc);
        fs->showModeless();
    }
    else
    {
        QStringList singlePathList;
        foreach (QString path, pathList)
        {
            singlePathList.clear();
            singlePathList << path;
            FileSelector *fs = new FileSelector(parent, svnAction, singlePathList, isFileList, wc);
            fs->showModeless();
        }
    }
}
