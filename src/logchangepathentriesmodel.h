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

#ifndef LOGCHANGEPATHENTRIESMODEL_H
#define LOGCHANGEPATHENTRIESMODEL_H

//SvnQt
#include "svnqt/client.hpp"
#include "svnqt/log_entry.hpp"

//Qt
#include <QAbstractTableModel>


/**
    @author Andreas Richter <ar@anrichter.net>
*/
class LogChangePathEntriesModel : public QAbstractTableModel
{
    public:
        LogChangePathEntriesModel(QObject *parent);
        void setChangePathEntries(svn::LogChangePathEntries logChangePathEntries);

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        QVariant data(const QModelIndex &index, int role) const;

        svn::LogChangePathEntry getLogChangePathEntry(const QModelIndex & index);

    private:
        svn::LogChangePathEntries m_logChangePathEntries;
};

#endif
