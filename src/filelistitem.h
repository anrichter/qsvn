/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2006 Andreas Richter <ar@oszine.de>                *
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


#ifndef FILELISTITEM_H
#define FILELISTITEM_H

//Qt
#include <QtGui>


class FileListItem
{
public:
    enum Columns {
        FilenameColumn = 0,
        StatusColumn = 1,
        RevisionColumn = 2,
        AuthorColumn = 3,
        FullfilenameColumn = 4,
        CheckedColumn = 5
    };

    FileListItem( const QList<QVariant> &data, FileListItem *parent = 0 );
    FileListItem( const FileListItem *fileListItem, FileListItem *parent = 0 );
    ~FileListItem();

    void appendChild( FileListItem *child );
    void removeChild( int row );

    FileListItem *child( int row );
    int childCount() const;
    int columnCount() const;
    QVariant data( int column ) const;
    void setData( int column, QVariant value );
    int row() const;
    FileListItem *parent();

    int getSortColumn();
    void setSortColumn( int column );
    bool operator < (const FileListItem &other) const;

    QPixmap getPixmap();

    QString fullFileName();
private:
    QList<FileListItem*> childItems;
    QList<QVariant> itemData;
    FileListItem *parentItem;
    int sortColumn;
};

#endif
