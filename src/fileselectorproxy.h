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

#ifndef FILESELECTORPROXY_H
#define FILESELECTORPROXY_H

//QSvn
#include "statusentriesmodel.h"
#include "svnclient.h"

//Qt
#include <QSortFilterProxyModel>


class FileSelectorProxy : public QSortFilterProxyModel
{
public:
    FileSelectorProxy( QObject *parent, SvnClient::SvnAction svnAction );
    ~FileSelectorProxy();

    void readDirectory( QString directory, const bool descend = false );
    void readFileList( QStringList fileList );

    svn::Status at( const QModelIndex &index );

    QVariant data( const QModelIndex &index, int role ) const;
    bool setData( const QModelIndex &index, const QVariant &value, int role );
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QStringList checkedFileList(); //return a list with full path from checked entries
    void setSelectAllState( int state );
protected:
    bool filterAcceptsRow ( int source_row, const QModelIndex &source_parent ) const;
private:
    StatusEntriesModel *m_statusEntriesModel;
    QSet<int> checkedRows;
    SvnClient::SvnAction m_svnAction;
};

#endif
