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
#include "fileselectorproxy.h"
#include "statusentriesmodel.h"
#include "svnclient.h"

//SvnQt
#include "svnqt/client.hpp"
#include "svnqt/status.hpp"

//Qt
#include <QtCore>
#include <QSortFilterProxyModel>

FileSelectorProxy::FileSelectorProxy(QObject *parent,
                                     SvnClient::SvnAction svnAction)
        : QSortFilterProxyModel(parent)
{
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
    }
    m_statusEntriesModel = new StatusEntriesModel(this);
    setSourceModel(m_statusEntriesModel);
}

void FileSelectorProxy::readDirectory(QString directory, const bool descend)
{
    m_statusEntriesModel->readDirectory(directory, descend, true);
    checkedRows.clear();
}

void FileSelectorProxy::readFileList(QStringList fileList)
{
    m_statusEntriesModel->readFileList(fileList);
    checkedRows.clear();
}

svn::Status FileSelectorProxy::at(const QModelIndex &index)
{
    return m_statusEntriesModel->at(mapToSource(index).row());
}

bool FileSelectorProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    svn::Status status = m_statusEntriesModel->at(source_row);
    return m_visibleStats.contains(status.textStatus());
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
        return m_statusEntriesModel->data(mapToSource(index), role);
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
        fileList << m_statusEntriesModel->at(row).path();
    }

    return fileList;
}

void FileSelectorProxy::setSelectAllState(int state)
{
    for (int i = 0; i < rowCount(); ++i)
        setData(index(i, 0), state, Qt::CheckStateRole);
}
