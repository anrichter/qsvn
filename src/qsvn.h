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
class FileListModel;
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
    void createActions();
    void createMenus();
    void createToolBar();

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

private slots:
    //File
    void exitSlot();

    //WorkingCopy
    void addWorkingCopySlot();
    void removeWorkingCopySlot();
    void checkoutSlot();

    //Modify
    void updateSlot();
    /*
        void commitSlot();
        void addSlot();
        void removeSlot();
        void revertSlot();

        //Query
        void diffSlot();
    */
    //Settings
    void configureQSvnSlot();
    //Help
    void helpContentsSlot();
    void helpIndexSlot();
    /*todo:
        void aboutSlot();
    todo */

    void activateWorkingCopy( const QModelIndex &index );
};

#endif
