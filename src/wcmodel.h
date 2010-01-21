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

#ifndef WCMODEL_H
#define WCMODEL_H

//Qt
#include <QStandardItemModel>


class WcModel : public QStandardItemModel
{
        Q_OBJECT

    public:
        WcModel(QObject *parent = 0);
        ~WcModel();

        bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

        void insertWc(QString dir);
        void removeWc(QString dir);
        void updateWc(QString dir);
        QString getPath(const QModelIndex &index) const;

    public slots:
        void doCollapse(const QModelIndex &index);

    private:
        enum UserRoles
        {
            PathRole = Qt::UserRole + 1,
            PopulatedRole = Qt::UserRole + 2
        };

        void insertDir(QString dir, QStandardItem *parent, int row) const;
        void populate(QStandardItem *parent) const;
        QStandardItem* itemFromDirectory(const QString dir, const QStandardItem* parent = 0);

        void saveWcList();
        void loadWcList();

};

#endif
