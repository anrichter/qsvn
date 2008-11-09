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

#ifndef STATUSENTRIESMODEL_H
#define STATUSENTRIESMODEL_H

//QSvn
#include "qarfilesystemwatcher.h"

//SvnQt
#include "svnqt/client.hpp"

//Qt
#include <QAbstractTableModel>


class StatusEntriesModel : public QAbstractTableModel
{
        Q_OBJECT

    public:
        StatusEntriesModel(QObject *parent);
        ~StatusEntriesModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        QVariant data(const QModelIndex &index, int role) const;

        void readDirectory(QString directory, svn::Depth depth, const bool force);
        void readFileList(QStringList fileList);

        svn::StatusPtr at(int row);
        void enableFsUpdates();
        void disableFsUpdates();
        void doFsUpdates();

    signals:
        void beginUpdate();
        void endUpdate();

    private:
        svn::StatusEntries m_statusEntries;
        QString m_directory;
        svn::Depth m_depth;
        QarFileSystemWatcher m_fsWatcher;
        bool m_isFsWatcherActive;
        bool m_existFsChanges;

        void clearFsWatcher();
        void fillFsWatcher();

        QPixmap statusPixmap(svn::StatusPtr status) const;
        QString statusString(svn_wc_status_kind status) const;

#if defined Q_WS_WIN32
        void checkCaseSensitivity();
#endif

    private slots:
        void onFsChanged();
};

#endif
