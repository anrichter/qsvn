/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2009 Andreas Richter <ar@anrichter.net>                 *
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

#ifndef QSVN_H
#define QSVN_H

//QSvn
#include "ui_qsvn.h"

class FileListProxy;

class StatusEntriesModel;

class WcModel;

//QT
#include <QMainWindow>


class QSvn : public QMainWindow, public Ui::QSvn
{
        Q_OBJECT

    public:
        QSvn(QWidget *parent = 0, Qt::WFlags flags = 0);
        ~QSvn();

        void updateWc(const QString dir);

    protected:
        bool eventFilter(QObject *watched, QEvent *event);
        void closeEvent(QCloseEvent *event);
        void changeEvent(QEvent *event);

    private:
        //ContextMenus
        QMenu *contextMenuWorkingCopy;
        QMenu *contextMenuFileList;

        WcModel *wcModel;
        FileListProxy *m_fileListProxy;
        StatusEntriesModel *m_statusEntriesModel;
        QString m_currentWCpath; //current working copy path

        void createMenus();

        bool isFileListSelected();
        QStringList selectedPaths();

        void setActionStop(QString aText);

    private slots:
        //WorkingCopy
        void on_actionWcAdd_triggered();
        void on_actionWcCheckout_triggered();
        void on_actionWcCleanup_triggered();
        void on_actionMkDir_triggered();
        void on_actionWcRemoveFromFavorites_triggered();
        void on_actionWcRemoveFromDisk_triggered();

        //FileList
        void on_actionFlResolved_triggered();
        void on_actionFlRename_triggered();
        void on_actionFlMove_triggered();
        void on_actionFlCopy_triggered();
        void on_actionFlRemoveFromDisk_triggered();

        //Both
        void on_actionUpdate_triggered();
        void on_actionCommit_triggered();
        void on_actionAdd_triggered();
        void on_actionDelete_triggered();
        void on_actionRevert_triggered();
        void on_actionShowLog_triggered();
        void on_actionMerge_triggered();
        void on_actionEditProperties_triggered();

        //Query
        void on_actionDiff_triggered();

        //Settings
        void on_actionConfigureQSvn_triggered();

        //Help
        void on_actionAboutQSvn_triggered();

        void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
        void directoryChanged(const QString &dir);

        //QSvnActionHandlers
        void onQSvnClientCheckoutActionFinished(QString path);
};

#endif
