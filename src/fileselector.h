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

#ifndef FILESELECTOR_H
#define FILESELECTOR_H


//QSvn
#include "ui_fileselector.h"
#include "svnclient.h"
class FileSelectorProxy;
class StatusEntriesModel;

//Qt
#include <QDialog>


class FileSelector : public QDialog, public Ui::FileSelector
{
        Q_OBJECT

    public:
        //static functions
        static void doSvnAction(QWidget *parent,
                                const SvnClient::SvnAction svnAction,
                                const QStringList pathList,
                                const bool isFileList,
                                const QString wc);

    protected:
        bool eventFilter(QObject *watched, QEvent *event);
        void changeEvent(QEvent *event);

    private:
        FileSelector(QWidget *parent,
                     const SvnClient::SvnAction svnAction,
                     const QStringList pathList,
                     const bool isFileList,
                     const QString wc);
        ~FileSelector();

        SvnClient::SvnAction m_svnAction;
        QItemSelectionModel *m_selectionModel;
        QMenu *contextMenu;
        StatusEntriesModel *m_statusEntriesModel;
        FileSelectorProxy *m_fileSelectorProxy;
        QString m_wc;

        void showModeless();
        void setupFileSelector(SvnClient::SvnAction svnAction);
        void setupUI();
        void setupMenus();
        void setupConnections();

    private slots:
        void accept();
        void on_comboLogHistory_activated(int index);
        void on_checkSelectAll_stateChanged(int state);
        void updateActions(const QItemSelection &selected,
                           const QItemSelection &deselected); //enable and disable some actions depend on what entry is selected

        void on_actionDiff_triggered();
        void on_actionRevert_triggered();
        void on_actionResolved_triggered();
        void on_actionShowLog_triggered();
};

#endif
