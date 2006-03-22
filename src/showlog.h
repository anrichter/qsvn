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
#ifndef SHOWLOG_H
#define SHOWLOG_H

//QSvn
#include "ui_showlog.h"
class LogEntriesModel;

//SvnQt
#include "svnqt/client.hpp"

//Qt
class QDialog;


/**
    @author Andreas Richter <ar@oszine.de>
*/

class ShowLog : public QDialog, public Ui::ShowLog
{
    Q_OBJECT

public:
    ShowLog( QWidget *parent = 0, const svn::LogEntries *logEntries = 0 );
    ~ShowLog();

private:
    LogEntriesModel *m_logEntriesModel;

private slots:
    void selectLogEntry( const QModelIndex &index );

};

#endif
