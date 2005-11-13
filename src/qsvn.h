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

#ifndef QSVN_H
#define QSVN_H

//QSvn
#include "ui_qsvn.h"
#include "filelistmodel.h"
#include "fileselector.h"
class WorkingCopyModel;


//QT
#include <QtCore>


class QSvn : public QMainWindow, public Ui::QSvn
{
    Q_OBJECT

public:
    QSvn( QWidget *parent = 0, Qt::WFlags flags = 0 );
    ~QSvn();

private:
    //Actions
    QAction* actionExit;
    QAction* actionHelpContents;
    QAction* actionHelpIndex;
    QAction* actionAboutQSvn;
    QAction* actionAddWorkingCopy;
    QAction* actionRemoveWorkingCopy;
    QAction* actionAboutQt;
    QAction* actionConfigureQSvn;
    QAction* actionUpdate;
    QAction* actionCommit;
    QAction* actionAdd;
    QAction* actionRemove;
    QAction* actionDiff;
    QAction* actionCheckout;
    QAction* actionRevert;

    //Menus
    QMenu *fileMenu;
    QMenu *workingCopyMenu;
    QMenu *modifyMenu;
    QMenu *queryMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;

    WorkingCopyModel *workingCopyModel;
    FileListModel *fileListModel;

    void createActions();
    void createMenus();
    void createToolBar();

    QItemSelectionModel *focusSelectionModel();
    FileListModel::FromSelectionType focusFromSelectionType();
private slots:
    //File
    void exit();

    //WorkingCopy
    void addWorkingCopy();
    void removeWorkingCopy();
    void checkout();

    //Modify
    void update();
    void commit();
    void add();
    void remove();
    void revert();

    //Query
    void diff();

    //Settings
    void configureQSvn();
    //Help
    void helpContents();
    void helpIndex();
    void aboutQSvn();

    void activateWorkingCopy( const QModelIndex &index );
};

#endif
