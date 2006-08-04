/***************************************************************************
 *   This file is part of QSvn Project http://ar.oszine.de/projects/qsvn   *
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
class LogChangePathEntriesModel;

//SvnQt
#include "svnqt/client.hpp"

//Qt
class QDialog;


class ShowLog : public QDialog, public Ui::ShowLog
{
    Q_OBJECT

public:
    ShowLog( QWidget *parent = 0, const QString path = 0, const svn::Revision revisionStart = svn::Revision::UNDEFINED, const svn::Revision revisionEnd = svn::Revision::UNDEFINED );
    ~ShowLog();

    static void doShowLog( QWidget *parent, const QString path, const svn::Revision revisionStart, const svn::Revision revisionEnd );

public slots:
    void addLogEntries();
    void addAllLogEntries();

protected:
    bool eventFilter ( QObject * watched, QEvent * event );

private:
    svn::LogEntries *m_logEntries;
    LogEntriesModel *m_logEntriesModel;
    LogChangePathEntriesModel *m_logChangePathEntriesModel;
    QMenu *contextLogChangePathEntries;
    QString m_path;
    svn::Revision m_revisionStart, m_revisionEnd, m_revisionBeginShowLog;

    void connectActions();
    void addLogEntries( int limit );

private slots:
	void selectionChanged( const QItemSelection &selected, const QItemSelection &deselected );
	void cbStrictNodeHistoryStateChanged();

    void doDiff();
};

#endif
