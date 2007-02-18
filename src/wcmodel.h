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

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

        void addWc(QString dir);
        void removeWc(const QModelIndex &index);
        QString getPath(const QModelIndex &index) const;

    private:
        void addDir(QString dir, QStandardItem *parent) const;

        void saveWcList();
        void loadWcList();

    public slots:
        void doUpdate(const QModelIndex &index);
};

#endif
