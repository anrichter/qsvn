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

#ifndef QARFILESYSTEMWATCHER_H
#define QARFILESYSTEMWATCHER_H

#include <QtCore>

class QarFileSystemWatcher : public QFileSystemWatcher
{
        Q_OBJECT

    public:
        void removeAllPaths();

#if defined Q_WS_WIN32
        QarFileSystemWatcher(QObject *parent = 0);
        ~QarFileSystemWatcher();

        void addPaths(const QStringList &paths);

    private:
        QList<QFileSystemWatcher*> m_fsWatcherList;
        QFileSystemWatcher *m_fsWatcher;

#endif

};


#endif
