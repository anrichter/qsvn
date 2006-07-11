/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2006 Andreas Richter <ar@oszine.de>                *
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


#ifndef FILESELECTOR_H
#define FILESELECTOR_H


//QSvn
#include "ui_fileselector.h"
#include "filelistmodel.h"
#include "svnclient.h"

//Qt
class QMenu;


class FileSelector : public QDialog, public Ui::FileSelector
{
    Q_OBJECT

public:
    FileSelector( QWidget *parent, SvnClient::SvnAction svnAction,
                  QItemSelectionModel *selectionModel,
                  FileListModel::SelectionFrom selectionFrom );
    ~FileSelector();

    FileListModel *model();
    QStringList selectedFileList();
    QString logMessage();

public slots:
    int exec();

protected:
    bool eventFilter( QObject *watched, QEvent *event );

private:
    FileListModel *m_fileListModel;
    SvnClient::SvnAction m_svnAction;
    QItemSelectionModel *m_selectionModel;
    FileListModel::SelectionFrom m_selectionFrom;
    QMenu *contextMenu;

    void initModel();
    void configUI();
    void createMenus();
    void setupConnections();
    void hideGroupBoxLogMessage();

private slots:
    void diff( const QModelIndex &index );
    void buttonOkClickedSlot();
    void comboLogHistoryActivatedSlot( int index );
    void checkSelectAllStateChanged( int state );

    void doDiff();
    void doRevert();
    
};

#endif
