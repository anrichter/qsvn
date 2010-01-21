/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2010 Andreas Richter <ar@anrichter.net>                 *
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

#ifndef FILESELECTORPROXY_H
#define FILESELECTORPROXY_H

//QSvn
#include "svnclient.h"

//Qt
#include <QSet>
#include <QSortFilterProxyModel>


class FileSelectorProxy : public QSortFilterProxyModel
{
        Q_OBJECT

    public:
        FileSelectorProxy(QObject *parent, SvnClient::SvnAction svnAction);

        QVariant data(const QModelIndex &index, int role) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role);
        Qt::ItemFlags flags(const QModelIndex &index) const;

        QStringList checkedFileList(); //return a list with full path from checked entries
        void setSelectAllState(int state);

    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    private:
        QSet<int> checkedRows;
        SvnClient::SvnAction m_svnAction;
        QSet<svn_wc_status_kind> m_visibleStats;
};

#endif
