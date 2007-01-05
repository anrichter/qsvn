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
    QSvn( QWidget *parent = 0, Qt::WFlags flags = 0 );
    ~QSvn();

protected:
    bool eventFilter( QObject * watched, QEvent * event );
    void closeEvent( QCloseEvent * event );

private:
    //ContextMenus
    QMenu *contextMenuWorkingCopy;
    QMenu *contextMenuFileList;

    WcModel *wcModel;
    FileListProxy *fileListProxy;
    QString m_currentWCpath; //current working copy path

    void setActionIcons();
    void connectActions();
    void createMenus();

    bool isFileListSelected();
    QStringList selectedPaths();

    void setActionStop( QString aText );

private slots:
    //WorkingCopy
    void doAddWorkingCopy();
    void doRemoveWorkingCopy();
    void doCheckoutWorkingCopy();

    //Modify
    void doUpdate();
    void doCommit();
    void doAdd();
    void doDelete();
    void doRevert();
    void doShowLog();
    void doCleanup();
    void doResolved();

    //Query
    void doDiff();

    //Settings
    void configureQSvn();

    //Help
    void aboutQSvn();

    void activateWorkingCopy( const QModelIndex &index, const bool force = false );
};

#endif
