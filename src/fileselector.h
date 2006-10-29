/***************************************************************************
 *   This file is part of QSvn Project http://ar.oszine.de/projects/qsvn   *
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
#include "fileselectorproxy.h"
#include "svnclient.h"

//Qt
class QMenu;


class FileSelector : public QDialog, public Ui::FileSelector
{
    Q_OBJECT

public:
    /**
     * Create a FileSelector for a working copy
     * @param parent the parent widget
     * @param svnAction the action the file selector is for
     * @param wc the working copy
     */
    FileSelector( QWidget *parent, SvnClient::SvnAction svnAction,
                  QString wc );
    ~FileSelector();

    QStringList checkedFileList();
    QString logMessage();

public slots:
    int exec();

protected:
    bool eventFilter( QObject *watched, QEvent *event );

private:
    SvnClient::SvnAction m_svnAction;
    QItemSelectionModel *m_selectionModel;
    QMenu *contextMenu;

    FileSelectorProxy *m_fileSelectorProxy;

    void configUI();
    void createMenus();
    void updateMenus(); //enable and disable some actions depend on what entry is selected
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
