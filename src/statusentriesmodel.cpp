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
#include "statusentriesmodel.h"
#include "svnclient.h"

//SvnQt
#include "svnqt/client.hpp"

//Qt
#include <QtCore>
#include <QtGui>


StatusEntriesModel::StatusEntriesModel(QObject *parent, QSet<svn_wc_status_kind> visibleStats)
        : QAbstractTableModel(parent)
{
    m_statusEntries = svn::StatusEntries();
	m_visibleStats = visibleStats;
    fillFsWatcher();
    connect(&m_fsWatcher, SIGNAL(fileChanged(const QString &)),
            this, SLOT(doFileChanged(const QString &)));
    connect(&m_fsWatcher, SIGNAL(directoryChanged(const QString &)),
            this, SLOT(doDirectoryChanged(const QString &)));
}

StatusEntriesModel::~StatusEntriesModel()
{
    clearFsWatcher();
}

int StatusEntriesModel::rowCount(const QModelIndex &parent) const
{
    return m_statusEntries.count();
}

int StatusEntriesModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant StatusEntriesModel::headerData(int section,
                                        Qt::Orientation orientation,
                                        int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
            case 0:
                return QString(tr("Filename"));
                break;
            case 1:
                return QString(tr("Status"));
                break;
            case 2:
                return QString(tr("Revision"));
                break;
            case 3:
                return QString(tr("Author"));
                break;
        }
    }
    return QVariant();
}

QVariant StatusEntriesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    svn::Status status = m_statusEntries.at(index.row());
    QFileInfo fileInfo(status.path());

    switch (role)
    {
        case Qt::DisplayRole:
            switch (index.column())
            {
                case 0:
                    if (!status.isVersioned() ||                         //return path for unversioned Files
                         (status.isVersioned() && fileInfo.isDir()) ||  //            and for versioned Directories
                         m_descend)
                    {
                        QString path = QDir::toNativeSeparators(status.path());
                        return path.remove(m_directory);
                    }
                    else
                        return status.entry().name();
                    break;
                case 1:
                    return statusString(status);
                    break;
                case 2:
                    if (status.isVersioned())
                        return int(status.entry().cmtRev());
                    break;
                case 3:
                    return status.entry().cmtAuthor();
                    break;
            }
            break;
        case Qt::DecorationRole:
            if (index.column() == 0)
                return statusPixmap(status);
            break;
    }
    return QVariant();
}

void StatusEntriesModel::readDirectory(QString directory, const bool descend,
                                       const bool force)
{
    directory = QDir::cleanPath(directory) + QDir::separator();
    directory = QDir::toNativeSeparators(directory);
    if (force || (m_directory != directory))
    {
        clearFsWatcher();
        m_descend = descend;
        m_directory = directory;
        m_statusEntries = SvnClient::instance()->status(m_directory, m_descend);
        fillFsWatcher();
        emit layoutChanged();
    }
}

void StatusEntriesModel::readFileList(QStringList fileList)
{
    clearFsWatcher();
    m_statusEntries.clear();

    foreach (QString file, fileList)
    m_statusEntries.append(SvnClient::instance()->singleStatus(file));

    fillFsWatcher();
}

svn::Status StatusEntriesModel::at(int row)
{
    return m_statusEntries.at(row);
}

QPixmap StatusEntriesModel::statusPixmap(svn::Status status) const
{
    switch (status.textStatus())
    {
        case svn_wc_status_none:
            return QPixmap(":/images/file.png");
        case svn_wc_status_unversioned:
            return QPixmap(":/images/unknownfile.png");
        case svn_wc_status_normal:
            return QPixmap(":/images/file.png");
        case svn_wc_status_added:
            return QPixmap(":/images/addedfile.png");
        case svn_wc_status_missing:
            return QPixmap(":/images/missingfile.png");
        case svn_wc_status_deleted:
            return QPixmap(":/images/modifiedfile.png");
        case svn_wc_status_replaced:
            return QPixmap(":/images/modifiedfile.png");
        case svn_wc_status_modified:
            return QPixmap(":/images/modifiedfile.png");
        case svn_wc_status_merged:
            return QPixmap(":/images/modifiedfile.png");
        case svn_wc_status_conflicted:
            return QPixmap(":/images/conflictedfile.png");
        case svn_wc_status_ignored:
            return QPixmap(":/images/file.png");
        case svn_wc_status_obstructed:
            return QPixmap(":/images/file.png");
        case svn_wc_status_external:
            return QPixmap(":/images/file.png");
        case svn_wc_status_incomplete:
            return QPixmap(":/images/file.png");
        default:
            return QPixmap(":/images/file.png");
    }
}

QString StatusEntriesModel::statusString(svn::Status status) const
{
    switch (status.textStatus())
    {
        case svn_wc_status_none:
            return QString(tr("none"));
        case svn_wc_status_unversioned:
            return QString(tr("unversioned"));
        case svn_wc_status_normal:
            return QString(tr("normal"));
        case svn_wc_status_added:
            return QString(tr("added"));
        case svn_wc_status_missing:
            return QString(tr("missing"));
        case svn_wc_status_deleted:
            return QString(tr("deleted"));
        case svn_wc_status_replaced:
            return QString(tr("replaced"));
        case svn_wc_status_modified:
            return QString(tr("modified"));
        case svn_wc_status_merged:
            return QString(tr("merged"));
        case svn_wc_status_conflicted:
            return QString(tr("conflicted"));
        case svn_wc_status_ignored:
            return QString(tr("ignored"));
        case svn_wc_status_obstructed:
            return QString(tr("obstructed"));
        case svn_wc_status_external:
            return QString(tr("external"));
        case svn_wc_status_incomplete:
            return QString(tr("incomplete"));
        default:
            return QString(status.textStatus());
    }
}

void StatusEntriesModel::doDirectoryChanged(const QString &path)
{
    if (SvnClient::instance()->isInProgress())
        return;

    readDirectory(m_directory, m_descend, true);
}

void StatusEntriesModel::doFileChanged(const QString &path)
{
    if (SvnClient::instance()->isInProgress())
        return;

    for (int i = 0; i < m_statusEntries.count(); i++)
    {
        if (m_statusEntries.at(i).path() == path)
        {
            svn::Status status = SvnClient::instance()->singleStatus(
                    m_statusEntries.at(i).path());
            if (status.isVersioned())
            {
                m_statusEntries.replace(i, status);
            }
            else {
                m_statusEntries.removeAt(i);
            }
            emit layoutChanged();
            break;
        }
    }
}

void StatusEntriesModel::clearFsWatcher()
{
    m_fsWatcher.removePaths(m_fsWatcher.directories());
    m_fsWatcher.removePaths(m_fsWatcher.files());
}

void StatusEntriesModel::fillFsWatcher()
{
    foreach(svn::Status status, m_statusEntries)
    {
        if (m_visibleStats.contains(status.textStatus()))
        {
            if (QFile::exists(status.path()))
                m_fsWatcher.addPath(status.path());
            else
            {
                QFileInfo fi(status.path());
                m_fsWatcher.addPath(fi.dir().path());
            }
        }
    }
}

#include "statusentriesmodel.moc"
