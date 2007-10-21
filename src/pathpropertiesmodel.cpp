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
#include "pathpropertiesmodel.h"
#include "svnclient.h"


PathPropertiesModel::PathPropertiesModel(const QString path)
        : QAbstractTableModel()
{

    //get the property map
    svn::PathPropertiesMapListPtr propList;
    propList = SvnClient::instance()->propList(path, svn::Revision::WORKING, svn::Revision::WORKING);
    if(!propList->isEmpty())
    {
        svn::PathPropertiesMapEntry entry = propList->at(0);
        m_propMap = entry.second;
    }
    else
    {
        m_propMap = svn::PropertiesMap();
    }
}

PathPropertiesModel::~PathPropertiesModel()
{
}

int PathPropertiesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_propMap.count();
}

int PathPropertiesModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant PathPropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
            case 0:
                return QString(tr("Name"));
                break;
            case 1:
                return QString(tr("Value"));
                break;
        }
    }
    return QVariant();
}

QVariant PathPropertiesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
            case 0:
                return m_propMap.keys().at(index.row());
                break;
            case 1:
                return m_propMap[m_propMap.keys().at(index.row())];
                break;
        }
    }
    else
        return QVariant();
}

Qt::ItemFlags PathPropertiesModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    else
        return QAbstractItemModel::flags(index);
}

bool PathPropertiesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::EditRole)
    {
        QString propValue;
        switch(index.column())
        {
            case 0:
                propValue = this->data(this->index(index.row(), 1), Qt::DisplayRole).toString();
                if (!m_propMap.contains(value.toString()))
                {
                    m_propMap.insert(value.toString(), propValue);
                }
                break;
            case 1:
                m_propMap[m_propMap.keys().at(index.row())] = value.toString();
                break;
        }
    }
    return true;
}
