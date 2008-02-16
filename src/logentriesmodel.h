/***************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2007 Andreas Richter <ar@anrichter.net>                *
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
#ifndef LOGENTRIESMODEL_H
#define LOGENTRIESMODEL_H

//SvnQt
#include "svnqt/client.hpp"

//Qt
#include <QAbstractTableModel>


/**
    @author Andreas Richter <ar@anrichter.net>
*/
class LogEntriesModel : public QAbstractTableModel
{
    public:
        LogEntriesModel(QObject *parent);
        ~LogEntriesModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        QVariant data(const QModelIndex &index, int role) const;

        void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
        svn::LogEntry getLogEntry(const QModelIndex & index);

        void appendLogEntries(const svn::LogEntriesPtr logEntries);
        void clear();

    private:
        svn::LogEntries m_logEntries;

        //sorting algorithms
        static bool logEntryLessThan(const svn::LogEntry &left, const svn::LogEntry &right);
        static bool logEntryGreaterThan(const svn::LogEntry &left, const svn::LogEntry &right);
};

#endif
