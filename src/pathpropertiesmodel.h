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

#ifndef PATHPROPERTIESMODEL_H
#define PATHPROPERTIESMODEL_H

//SvnQt
#include "svnqt/client.hpp"
#include "svnqt/svnqttypes.hpp"

//Qt
#include <QAbstractTableModel>
#include <QItemSelectionModel>


class PathPropertiesModel : public QAbstractTableModel
{

    public:
        PathPropertiesModel(QObject *parent, const QString path);
        ~PathPropertiesModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        QVariant data(const QModelIndex &index, int role) const;

        void addProperty(const QString propertyName, const QString propertyValue);
        void setProperty(const QString propertyName, const QString propertyValue);
        void deleteProperty(const QItemSelectionModel &selection);
        void writeProperties();
        QString getPropertyName(int row);
        QString getPropertyValue(int row);

    private:
        QString m_path;
        svn::PropertiesMap m_propMap;
};

#endif
