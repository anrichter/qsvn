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
#include "logchangepathentriesmodel.h"
#include "logchangepathentriesmodel.moc"

//SvnQt
#include "svnqt/client.hpp"
#include "svnqt/log_entry.hpp"

//Qt
#include <QtCore>
#include <QColor>


LogChangePathEntriesModel::LogChangePathEntriesModel(QObject *parent, const QString path)
        : QAbstractTableModel(parent)
{
    m_logChangePathEntries = svn::LogChangePathEntries();
    m_path = path;
}


void LogChangePathEntriesModel::setChangePathEntries(svn::LogChangePathEntries logChangePathEntries)
{
    m_logChangePathEntries = logChangePathEntries;
    emit layoutChanged();
}

int LogChangePathEntriesModel::rowCount(const QModelIndex &parent) const
{
    return m_logChangePathEntries.count();
}

int LogChangePathEntriesModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant LogChangePathEntriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
            case 0:
                return QString(tr("Action"));
                break;
            case 1:
                return QString(tr("Path"));
                break;
            case 2:
                return QString(tr("Copy from path"));
                break;
            case 3:
                return QString(tr("Copy from Revision"));
                break;
        }
    }
    return QVariant();
}

QVariant LogChangePathEntriesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    svn::LogChangePathEntry logChangePathEntry = m_logChangePathEntries.at(index.row());
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 0:
                return QChar(logChangePathEntry.action);
                break;
            case 1:
                return logChangePathEntry.path;
                break;
            case 2:
                return logChangePathEntry.copyFromPath;
                break;
            case 3:
                if (SVN_IS_VALID_REVNUM(logChangePathEntry.copyFromRevision))
                    return int(logChangePathEntry.copyFromRevision);
                else
                    return QVariant();
                break;
            default:
                return QVariant();
                break;
        }
    }
    else if (role == Qt::ForegroundRole)
    {
        if (logChangePathEntry.path.contains(m_path) )
            return QColor(Qt::black);
        else
            return QColor(Qt::gray);
    }
    else
        return QVariant();
}

svn::LogChangePathEntry LogChangePathEntriesModel::getLogChangePathEntry(const QModelIndex &index)
{
    return m_logChangePathEntries.at(index.row());
}
