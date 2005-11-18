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
 ***************************************************************************/


#ifndef WORKINGCOPYITEM_H
#define WORKINGCOPYITEM_H

//Qt
#include <QtCore>


class WorkingCopyItem
{
public:
    WorkingCopyItem( const QList<QVariant> &data, WorkingCopyItem *parent = 0 );
    ~WorkingCopyItem();

    void appendChild( WorkingCopyItem *child );
    void removeChild( int row );
    void deleteAllChilds();

    WorkingCopyItem *child( int row );
    int childCount() const;
    int columnCount() const;
    QVariant data( int column ) const;
    int row() const;
    WorkingCopyItem *parent();

    QPixmap getPixmap();
    QString fullPath() const;
    QStringList fullPathListRecursive( QStringList fullPathList = QStringList() );

private:
    QList<WorkingCopyItem*> childItems;
    QList<QVariant> itemData;
    WorkingCopyItem *parentItem;

    bool svnDirectory;
};

#endif
