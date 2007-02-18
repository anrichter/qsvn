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

#ifndef QSVN_H
#define QSVN_H

//QSvn
#include "ui_qsvn.h"
#include "svnclient.h"

class WcModel;
class FileListProxy;


//QT
#include <QtCore>


class QSvn : public QMainWindow, public Ui::QSvn
{
        Q_OBJECT

    public:
        QSvn ( QWidget *parent = 0, Qt::WFlags flags = 0 );
        ~QSvn();

    protected:
        bool eventFilter ( QObject * watched, QEvent * event );
        void closeEvent ( QCloseEvent * event );

    private:
        //ContextMenus
        QMenu *contextMenuWorkingCopy;
        QMenu *contextMenuFileList;

        WcModel *wcModel;
        FileListProxy *fileListProxy;
        QString m_currentWCpath; //current working copy path

        void connectActions();
        void createMenus();

        bool isFileListSelected();
        QStringList selectedPaths();

        void setActionStop ( QString aText );

    private slots:
        //WorkingCopy
        void on_actionAddWorkingCopy_triggered();
        void on_actionRemoveWorkingCopy_triggered();
        void on_actionCheckoutWorkingCopy_triggered();

        //Modify
        void on_actionUpdate_triggered();
        void on_actionCommit_triggered();
        void on_actionAdd_triggered();
        void on_actionDelete_triggered();
        void on_actionRevert_triggered();
        void on_actionLog_triggered();
        void on_actionCleanup_triggered();
        void on_actionResolved_triggered();
        void on_actionRename_triggered();
        void on_actionMove_triggered();
        void on_actionCopy_triggered();
        void on_actionMkDir_triggered();

        //Query
        void on_actionDiff_triggered();

        //Settings
        void on_actionConfigureQSvn_triggered();

        //Help
        void on_actionAboutQSvn_triggered();

        void activateWorkingCopy ( const QModelIndex &index, const bool force = false );
};

#endif
