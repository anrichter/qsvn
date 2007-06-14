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
#include "logentriesmodel.h"
#include "logchangepathentriesmodel.h"
#include "showlog.h"
#include "svnclient.h"

//svnqt
#include "svnqt/client.hpp"
#include "svnqt/log_entry.hpp"
#include "svnqt/wc.hpp"

//Qt
#include <QtGui>


ShowLog::ShowLog(QWidget *parent, const QString path,
                 const svn::Revision revisionStart,
                 const svn::Revision revisionEnd)
        : QDialog(0) //don't set parent here! FileSelector is always a top-level window
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_revisionStart = revisionStart;
    m_revisionEnd = revisionEnd;
    m_revisionBeginShowLog = revisionStart;

    setupUi(this);

    Config::instance()->restoreWidget(this);
    Config::instance()->restoreSplitter(this, splitter);

    m_logEntriesModel = new LogEntriesModel(this);
    m_logEntriesProxy = new QSortFilterProxyModel(this);
    m_logEntriesProxy->setDynamicSortFilter(true);
    m_logEntriesProxy->setSourceModel(m_logEntriesModel);


    m_logChangePathEntriesModel = new LogChangePathEntriesModel(this);
    m_logChangePathEntriesProxy = new QSortFilterProxyModel(this);
    m_logChangePathEntriesProxy->setDynamicSortFilter(true);
    m_logChangePathEntriesProxy->setSourceModel(m_logChangePathEntriesModel);

    viewLogEntries->setModel(m_logEntriesProxy);
    Config::instance()->restoreHeaderView(this, viewLogEntries->header());
    connect(viewLogEntries->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(selectionChanged(const QItemSelection &, const QItemSelection &)));

    viewLogChangePathEntries->setModel(m_logChangePathEntriesProxy);
    viewLogChangePathEntries->installEventFilter(this);
    Config::instance()->restoreHeaderView(this, viewLogChangePathEntries->header());
    connect(viewLogChangePathEntries, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(on_actionDiff_triggered()));

    contextLogChangePathEntries = new QMenu(this);
    contextLogChangePathEntries->addAction(actionDiff);

    m_path = path;
    m_path.replace("\\", "/");
    if (!m_path.endsWith("/"))
        m_path.append("/");

    this->setWindowTitle(QString(tr("Show Log for %1")).arg(path));
}

ShowLog::~ShowLog()
{
    Config::instance()->saveWidget(this);
    Config::instance()->saveSplitter(this, splitter);
    Config::instance()->saveHeaderView(this, viewLogEntries->header());
    Config::instance()->saveHeaderView(this, viewLogChangePathEntries->header());
}

void ShowLog::doShowLog(QWidget *parent, const QString path,
                        const svn::Revision revisionStart,
                        const svn::Revision revisionEnd )
{
    ShowLog *showLog = new ShowLog(parent, path, revisionStart, revisionEnd);
    showLog->show();
    showLog->raise();
    showLog->activateWindow();
    showLog->on_buttonNext_clicked();
}

void ShowLog::on_buttonNext_clicked()
{
    buttonNextClicked(100);
}

void ShowLog::on_buttonShowAll_clicked()
{
    buttonNextClicked(0);
}

void ShowLog::buttonNextClicked(int limit)
{
    qApp->processEvents();
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    m_logEntriesModel->appendLogEntries(
            SvnClient::instance()->log(m_path, m_revisionStart, m_revisionEnd, true,
                                       (checkBoxStrictNodeHistory->checkState() == Qt::Checked),
                                       limit));
    m_revisionStart = m_logEntriesModel->getLogEntry(
            m_logEntriesModel->index(m_logEntriesModel->rowCount() - 1, 0)).revision;

    buttonNext->setEnabled(m_revisionStart.revnum() > m_revisionEnd.revnum());
    buttonShowAll->setEnabled(buttonNext->isEnabled());

    QApplication::restoreOverrideCursor();
}

bool ShowLog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == viewLogChangePathEntries)
    {
        if (event->type() == QEvent::ContextMenu)
        {
            contextLogChangePathEntries->popup(static_cast<QContextMenuEvent*>(event)->globalPos());
        }
    }
    return QDialog::eventFilter(watched, event);
}

void ShowLog::selectionChanged(const QItemSelection &selected,
                               const QItemSelection &deselected)
{
    QModelIndex index = selected.indexes().at(0);
    if (index.isValid())
    {
        editLogMessage->setPlainText(m_logEntriesModel->getLogEntry(index).message);
        m_logChangePathEntriesModel->setChangePathEntries(m_logEntriesModel->getLogEntry(index).changedPaths);
    }
}

void ShowLog::on_checkBoxStrictNodeHistory_stateChanged()
{
    m_logEntriesModel->clear();
    m_revisionStart = m_revisionBeginShowLog;
    on_buttonNext_clicked();
}

void ShowLog::on_actionDiff_triggered()
{
    svn::LogEntry logEntry;
    svn::LogChangePathEntry logChangePathEntry;

    QModelIndexList indexes;
    indexes = viewLogEntries->selectionModel()->selectedIndexes();
    if (indexes.count() == 0)
        return;
    logEntry = m_logEntriesModel->getLogEntry(m_logEntriesProxy->mapToSource(indexes.at(0)));

    indexes = viewLogChangePathEntries->selectionModel()->selectedIndexes();
    if (indexes.count() == 0)
        return;
    logChangePathEntry = m_logChangePathEntriesModel->getLogChangePathEntry(
            m_logChangePathEntriesProxy->mapToSource(indexes.at(0)));

    SvnClient::instance()->diff(QString(svn::Wc::getRepos(m_path) + logChangePathEntry.path),
                                svn::Revision(logEntry.revision - 1),
                                svn::Revision(logEntry.revision));
}

#include "showlog.moc"
