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
#include "svnqt/status.hpp"

//Qt
#include <QtCore>
#include <QtGui>


StatusEntriesModel::StatusEntriesModel(QObject *parent)
        : QAbstractTableModel(parent)
{
    m_statusEntries = svn::StatusEntries();
    enableFsWatcher();
    connect(&m_fsWatcher, SIGNAL(directoryChanged(const QString &)),
            this, SLOT(doDirectoryChanged(const QString &)));
}

StatusEntriesModel::~StatusEntriesModel()
{
    disableFsWatcher();
}

int StatusEntriesModel::rowCount(const QModelIndex &parent) const
{
    return m_statusEntries.count();
}

int StatusEntriesModel::columnCount(const QModelIndex &parent) const
{
    return 5;
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
                return QString(tr("Text-Status"));
                break;
            case 2:
                return QString(tr("Property-Status"));
                break;
            case 3:
                return QString(tr("Revision"));
                break;
            case 4:
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

    svn::StatusPtr status = m_statusEntries.at(index.row());
    QFileInfo fileInfo(status->path());

    switch (role)
    {
        case Qt::DisplayRole:
            switch (index.column())
            {
                case 0: //FileName
                    if (!status->isVersioned() ||                         //return path for unversioned Files
                         (status->isVersioned() && fileInfo.isDir()) ||  //            and for versioned Directories
                         m_descend)
                    {
                        QString path = QDir::toNativeSeparators(status->path());
                        return path.remove(m_directory);
                    }
                    else
                        return status->entry().name();
                    break;
                case 1: //Text-Status
                    return statusString(status->textStatus());
                    break;
                case 2: //Property-Status
                    return statusString(status->propStatus());
                    break;
                case 3: //Revision
                    if (status->isVersioned())
                        return int(status->entry().cmtRev());
                    break;
                case 4: //Author
                    return status->entry().cmtAuthor();
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
        disableFsWatcher();
        m_descend = descend;
        m_directory = directory;
        m_statusEntries = SvnClient::instance()->status(m_directory, m_descend);
        enableFsWatcher();
        emit layoutChanged();
    }
}

void StatusEntriesModel::readFileList(QStringList fileList)
{
    disableFsWatcher();
    m_statusEntries.clear();

    foreach (QString file, fileList)
    m_statusEntries.append(SvnClient::instance()->singleStatus(file));

    enableFsWatcher();
}

svn::StatusPtr StatusEntriesModel::at(int row)
{
    return m_statusEntries.at(row);
}

QPixmap StatusEntriesModel::statusPixmap(svn::StatusPtr status) const
{
    svn_wc_status_kind _status = status->textStatus();
    if (_status == svn_wc_status_normal)
        _status = status->propStatus();

    switch (_status)
    {
        case svn_wc_status_unversioned:
            return QPixmap(":/images/unknownfile.png");
            break;
        case svn_wc_status_added:
            return QPixmap(":/images/addedfile.png");
            break;
        case svn_wc_status_missing:
            return QPixmap(":/images/missingfile.png");
            break;
        case svn_wc_status_deleted:
        case svn_wc_status_replaced:
        case svn_wc_status_modified:
        case svn_wc_status_merged:
            return QPixmap(":/images/modifiedfile.png");
            break;
        case svn_wc_status_conflicted:
            return QPixmap(":/images/conflictedfile.png");
            break;
        default:
            return QPixmap(":/images/file.png");
            break;
    }
}

QString StatusEntriesModel::statusString(svn_wc_status_kind status) const
{
    switch (status)
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
            return QString(status);
    }
}

void StatusEntriesModel::doDirectoryChanged(const QString &path)
{
    if (SvnClient::instance()->isInProgress())
        return;

    readDirectory(m_directory, m_descend, true);
}

void StatusEntriesModel::disableFsWatcher()
{
    if (!m_fsWatcher.directories().isEmpty())
        m_fsWatcher.removePaths(m_fsWatcher.directories());
}

void StatusEntriesModel::enableFsWatcher()
{
    foreach(svn::StatusPtr status, m_statusEntries)
        m_fsWatcher.addPath(QFileInfo(status->path()).absolutePath());
}

#include "statusentriesmodel.moc"
