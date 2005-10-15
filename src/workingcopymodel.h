/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2005 Andreas Richter <ar@oszine.de>                *
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
 *   As a special exception, permission is given to link this program      *
 *   with any edition of Qt, and distribute the resulting executable,      *
 *   without including the source code for Qt in the source distribution.  *
 ***************************************************************************/


#ifndef WORKINGCOPYMODEL_H
#define WORKINGCOPYMODEL_H

//QSvn
class WorkingCopyItem;

//Qt
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>


class WorkingCopyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles { FullDirectory = Qt::UserRole + 1 };

    WorkingCopyModel( QObject *parent = 0 );
    ~WorkingCopyModel();

    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex &index ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QVariant data( const QModelIndex &index, int role ) const;

    bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() );

    void addWorkingCopy( QString directory );

private:
    WorkingCopyItem *rootItem;

    void updateWorkingCopy( WorkingCopyItem *item );

    void saveWorkingCopies();
    void loadWorkingCopies();
};

#endif
