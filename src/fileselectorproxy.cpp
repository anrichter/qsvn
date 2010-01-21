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

//QSvn
#include "fileselectorproxy.h"
#include "fileselectorproxy.moc"
#include "statusentriesmodel.h"
#include "svnclient.h"

//SvnQt
#include "svnqt/status.hpp"

//Qt
#include <QSortFilterProxyModel>


FileSelectorProxy::FileSelectorProxy(QObject *parent,
                                     SvnClient::SvnAction svnAction)
        : QSortFilterProxyModel(parent)
{
    #if defined Q_WS_WIN32
	setSortCaseSensitivity(Qt::CaseInsensitive);
	#endif
    m_svnAction = svnAction;
    switch (m_svnAction)
    {
        case SvnClient::SvnAdd:
            m_visibleStats << svn_wc_status_unversioned;
            break;
        case SvnClient::SvnCommit:
            m_visibleStats << svn_wc_status_modified;
            m_visibleStats << svn_wc_status_added;
            m_visibleStats << svn_wc_status_deleted;
            m_visibleStats << svn_wc_status_replaced;
            m_visibleStats << svn_wc_status_conflicted;
            break;
        case SvnClient::SvnDelete:
            m_visibleStats << svn_wc_status_normal;
            m_visibleStats << svn_wc_status_merged;
            break;
        case SvnClient::SvnRevert:
            m_visibleStats << svn_wc_status_added;
            m_visibleStats << svn_wc_status_conflicted;
            m_visibleStats << svn_wc_status_deleted;
            m_visibleStats << svn_wc_status_modified;
            m_visibleStats << svn_wc_status_replaced;
            break;
        case SvnClient::RemoveFromDisk:
            m_visibleStats << svn_wc_status_unversioned
                    << svn_wc_status_normal
                    << svn_wc_status_modified
                    << svn_wc_status_merged
                    << svn_wc_status_conflicted
                    << svn_wc_status_ignored
                    << svn_wc_status_obstructed
                    << svn_wc_status_external
                    << svn_wc_status_incomplete;
        default:
            break;
    }
}

bool FileSelectorProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    svn::StatusPtr status = static_cast<StatusEntriesModel*>(sourceModel())->at(source_row);
    return m_visibleStats.contains(status->textStatus()) || m_visibleStats.contains(status->propStatus());
}

QVariant FileSelectorProxy::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((role == Qt::CheckStateRole) && (index.column() == 0))
    {
        if (checkedRows.contains(mapToSource(index).row()))
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }
    else
    {
        return sourceModel()->data(mapToSource(index), role);
    }
}

bool FileSelectorProxy::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::CheckStateRole)
    {
        if (value == Qt::Checked)
            checkedRows.insert(mapToSource(index).row());
        else
            checkedRows.remove(mapToSource(index).row());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags FileSelectorProxy::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable;
}

QStringList FileSelectorProxy::checkedFileList()
{
    QStringList fileList;
    QModelIndex index;

    foreach (int row, checkedRows)
    {
        fileList << static_cast<StatusEntriesModel*>(sourceModel())->at(row)->path();
    }

    return fileList;
}

void FileSelectorProxy::setSelectAllState(int state)
{
    for (int i = 0; i < rowCount(); ++i)
        setData(index(i, 0), state, Qt::CheckStateRole);
}
