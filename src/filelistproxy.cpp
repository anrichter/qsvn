/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2009 Andreas Richter <ar@anrichter.net>                 *
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
#include "filelistproxy.h"
#include "filelistproxy.moc"
#include "statusentriesmodel.h"

//SvnQt
#include "svnqt/status.hpp"

//Qt
#include <QtCore>
#include <QSortFilterProxyModel>

FileListProxy::FileListProxy(QObject *parent)
        : QSortFilterProxyModel(parent)
{
    #if defined Q_WS_WIN32
	setSortCaseSensitivity(Qt::CaseInsensitive);
	#endif
}

bool FileListProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    svn::StatusPtr status = static_cast<StatusEntriesModel*>(sourceModel())->at(source_row);
    if (QFileInfo(status->path()).isDir()) // don't show any directories in filelist
        return false;
    else
        return true;
}
