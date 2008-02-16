/***************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2007 Andreas Richter <ar@anrichter.net>                *
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
#include "statustext.h"

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

    //initLogEntries
    m_logEntriesModel = new LogEntriesModel(this);
    m_logEntriesProxy = new QSortFilterProxyModel(this);
    m_logEntriesProxy->setDynamicSortFilter(true);
    m_logEntriesProxy->setSourceModel(m_logEntriesModel);
    for (int i = 0; i < m_logEntriesModel->columnCount(QModelIndex()); i++)
        comboBoxFilterKeyColumn->insertItem(comboBoxFilterKeyColumn->count(),
                                            m_logEntriesModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
    comboBoxFilterKeyColumn->setCurrentIndex(Config::instance()->value("comboBoxFilterKeyColumn").toInt());
    connect(editFilterString, SIGNAL(textChanged(const QString &)),
            m_logEntriesProxy, SLOT(setFilterFixedString(const QString &)));

    viewLogEntries->setModel(m_logEntriesProxy);
    Config::instance()->restoreHeaderView(this, viewLogEntries->header());
    connect(viewLogEntries->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(selectionChanged(const QItemSelection &, const QItemSelection &)));

    //initLogEntriesPath
    m_logChangePathEntriesModel = new LogChangePathEntriesModel(this);
    m_logChangePathEntriesProxy = new QSortFilterProxyModel(this);
    m_logChangePathEntriesProxy->setDynamicSortFilter(true);
    m_logChangePathEntriesProxy->setSourceModel(m_logChangePathEntriesModel);

    viewLogChangePathEntries->setModel(m_logChangePathEntriesProxy);
    viewLogChangePathEntries->installEventFilter(this);
    viewLogChangePathEntries->sortByColumn(1, Qt::AscendingOrder);
    Config::instance()->restoreHeaderView(this, viewLogChangePathEntries->header());
    connect(viewLogChangePathEntries, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(on_actionDiff_triggered()));

    menuPathEntries = new QMenu(this);
    menuPathEntries->addAction(actionDiff);
    QMenu *menuPathEntriesDiff = menuPathEntries->addMenu(tr("Show differences against..."));
    menuPathEntriesDiff->addAction(actionDiff_to_WORKING);
    menuPathEntriesDiff->addAction(actionDiff_to_HEAD);
    menuPathEntriesDiff->addAction(actionDiff_to_BASE);
    menuPathEntriesDiff->addAction(actionDiff_to_START);
    menuPathEntriesDiff->addAction(actionDiff_to_Revision);

    m_path = path;
    m_path.replace("\\", "/");
    if (!m_path.endsWith("/"))
        m_path.append("/");

    this->setWindowTitle(QString(tr("Show Log for %1")).arg(path));
    editFilterString->setFocus(Qt::MouseFocusReason);
}

ShowLog::~ShowLog()
{
    Config::instance()->saveWidget(this);
    Config::instance()->saveSplitter(this, splitter);
    Config::instance()->saveHeaderView(this, viewLogEntries->header());
    Config::instance()->saveHeaderView(this, viewLogChangePathEntries->header());
    Config::instance()->setValue("comboBoxFilterKeyColumn", comboBoxFilterKeyColumn->currentIndex());
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
    setEnabled(false);
    qApp->processEvents();
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    m_logEntriesModel->appendLogEntries(
            SvnClient::instance()->log(m_path, m_revisionStart, m_revisionEnd, true,
                                       (checkBoxStrictNodeHistory->checkState() == Qt::Checked),
                                       limit));
    m_revisionStart = m_logEntriesModel->getLogEntry(
            m_logEntriesModel->index(m_logEntriesModel->rowCount() - 1, 0)).revision;

    buttonNext->setEnabled(m_revisionStart.revnum() > m_revisionEnd.revnum());
    buttonShowAll->setEnabled(m_revisionStart.revnum() > m_revisionEnd.revnum());

    QApplication::restoreOverrideCursor();
    setEnabled(true);
    qApp->processEvents();
}

bool ShowLog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == viewLogChangePathEntries)
    {
        if (event->type() == QEvent::ContextMenu)
        {
            menuPathEntries->popup(static_cast<QContextMenuEvent*>(event)->globalPos());
        }
    }
    return QDialog::eventFilter(watched, event);
}

void ShowLog::selectionChanged(const QItemSelection &selected,
                               const QItemSelection &deselected)
{
    if (selected.isEmpty())
    {
        editLogMessage->clear();
        m_logChangePathEntriesModel-> setChangePathEntries(svn::LogChangePathEntries());
    } else {
        QModelIndex index = m_logEntriesProxy->mapToSource(selected.indexes().at(0));
        if (index.isValid())
        {
            editLogMessage->setPlainText(m_logEntriesModel->getLogEntry(index).message);
            m_logChangePathEntriesModel->setChangePathEntries(m_logEntriesModel->getLogEntry(index).changedPaths);
        }
    }
}

void ShowLog::on_checkBoxStrictNodeHistory_stateChanged()
{
    viewLogEntries->clearSelection();
    m_logEntriesModel->clear();
    m_revisionStart = m_revisionBeginShowLog;
    on_buttonNext_clicked();
}

svn::Revision ShowLog::getSelectedRevision()
{
    svn::LogEntry logEntry;
    QModelIndexList indexes;
    indexes = viewLogEntries->selectionModel()->selectedIndexes();
    if (indexes.count() == 0)
        return svn::Revision(svn::Revision::UNDEFINED);
    logEntry = m_logEntriesModel->getLogEntry(m_logEntriesProxy->mapToSource(indexes.at(0)));
    return logEntry.revision;
}

QString ShowLog::getSelectedPath()
{
    svn::LogChangePathEntry logChangePathEntry;
    QModelIndexList indexes;
    indexes = viewLogChangePathEntries->selectionModel()->selectedIndexes();
    if (indexes.count() == 0)
        return QString();
    logChangePathEntry = m_logChangePathEntriesModel->getLogChangePathEntry(
            m_logChangePathEntriesProxy->mapToSource(indexes.at(0)));
    return logChangePathEntry.path;
}

QString ShowLog::getWcRootPath()
{
    QString _result = QDir::toNativeSeparators(m_path);
    while (svn::Wc::checkWc(_result.left(_result.lastIndexOf(QDir::separator()))))
        _result = _result.left(_result.lastIndexOf(QDir::separator()));
    return _result;
}

QString ShowLog::getWcRootDirPath()
{
    QString _result = svn::Wc::getUrl(getWcRootPath());
    _result.remove(svn::Wc::getRepos(getWcRootPath()));
    return _result;
}

QString ShowLog::getWcFilePath()
{
    QString _result = getWcRootPath() + getSelectedPath();
    _result.remove(getWcRootDirPath());
    _result = QDir::toNativeSeparators(_result);
    return _result;
}

svn_revnum_t ShowLog::getSelectedStartRevision()
{
    svn::LogEntriesPtr _logEntries;
    _logEntries = SvnClient::instance()->log(svn::Wc::getRepos(m_path) + getSelectedPath(),
                                             getSelectedRevision(), svn::Revision::START,
                                             true, true, 0);
    return _logEntries->first().revision;
}

bool ShowLog::checkLocatedInWc()
{
    if (getSelectedPath().startsWith(getWcRootDirPath()))
        return true;
    else
    {
        StatusText::out(tr("This file is not located in the working copy."));
        return false;
    }
}

void ShowLog::on_actionDiff_triggered()
{
    SvnClient::instance()->diff(svn::Wc::getRepos(m_path) + getSelectedPath(),
                                svn::Wc::getRepos(m_path) + getSelectedPath(),
                                svn::Revision(getSelectedRevision().revnum() - 1),
                                getSelectedRevision());
}

void ShowLog::on_actionDiff_to_WORKING_triggered()
{
    if (checkLocatedInWc())
    {
        SvnClient::instance()->diff(svn::Wc::getRepos(getWcRootPath()) + getSelectedPath(),
                                    getWcFilePath(),
                                    getSelectedRevision(),
                                    svn::Revision::WORKING);
    }
}

void ShowLog::on_actionDiff_to_HEAD_triggered()
{
    SvnClient::instance()->diff(svn::Wc::getRepos(getWcRootPath()) + getSelectedPath(),
                                svn::Wc::getRepos(getWcRootPath()) + getSelectedPath(),
                                getSelectedRevision(),
                                svn::Revision::HEAD);
}

void ShowLog::on_actionDiff_to_BASE_triggered()
{
    if (checkLocatedInWc())
    {
        SvnClient::instance()->diff(svn::Wc::getRepos(getWcRootPath()) + getSelectedPath(),
                                    getWcFilePath(),
                                    getSelectedRevision(),
                                    svn::Revision::BASE);
    }
}

void ShowLog::on_actionDiff_to_START_triggered()
{
    SvnClient::instance()->diff(svn::Wc::getRepos(getWcRootPath()) + getSelectedPath(),
                                svn::Wc::getRepos(getWcRootPath()) + getSelectedPath(),
                                getSelectedRevision(),
                                getSelectedStartRevision());
}

void ShowLog::on_actionDiff_to_Revision_triggered( )
{
    int revision = QInputDialog::getInteger(this,
                                            tr("Revision Number"),
                                            tr("Revision Number"),
                                            getSelectedRevision().revnum() - 1);
    SvnClient::instance()->diff(svn::Wc::getRepos(getWcRootPath()) + getSelectedPath(),
        svn::Wc::getRepos(getWcRootPath()) + getSelectedPath(),
        getSelectedRevision(),
        svn::Revision(revision));
}
void ShowLog::on_comboBoxFilterKeyColumn_currentIndexChanged(int index)
{
    m_logEntriesProxy->setFilterKeyColumn(index);
}


#include "showlog.moc"
