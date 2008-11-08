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

#include <qarfilesystemwatcher.h>

#include <QFileSystemWatcher>
#include <QtCore>

#if defined Q_WS_WIN32
#include <windows.h>

QarFileSystemWatcher::QarFileSystemWatcher(QObject *parent)
    : QFileSystemWatcher(parent)
{
    m_fsWatcher = this;
}

QarFileSystemWatcher::~QarFileSystemWatcher()
{
    qDeleteAll(m_fsWatcherList);
}

void QarFileSystemWatcher::addPaths(const QStringList &paths)
{
    int _count = m_fsWatcher->directories().count() +
            m_fsWatcher->files().count();
    QStringList _paths;

    foreach(QString path, paths)
    {
        _count++;
        _paths << path;

        if (_count > MAXIMUM_WAIT_OBJECTS)
        {
            m_fsWatcher->addPaths(_paths);
            m_fsWatcher = new QFileSystemWatcher();
            connect(m_fsWatcher, SIGNAL(directoryChanged(const QString &)),
                this, SIGNAL(directoryChanged(const QString &)));
            connect(m_fsWatcher, SIGNAL(fileChanged(const QString &)),
                this, SIGNAL(fileChanged(const QString &)));
            m_fsWatcherList.append(m_fsWatcher);
            _paths.clear();
            _count = 0;
        }
    }
    if (!_paths.isEmpty())
        m_fsWatcher->addPaths(_paths);
}
#endif

void QarFileSystemWatcher::removeAllPaths()
{
#if defined Q_WS_WIN32
    qDeleteAll(m_fsWatcherList);
    m_fsWatcherList.clear();
    m_fsWatcher = this;
#endif
    removePaths(directories());
    removePaths(files());
}
