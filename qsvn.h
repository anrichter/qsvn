/***************************************************************************
 *   Copyright (C) 2004 by Andreas Richter                                 *
 *   ar@oszine.de                                                          *
 *   http://www.oszine.de                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
 ***************************************************************************/

#ifndef QSVN_H
#define QSVN_H

#include "qsvndlg.h"

//Qt-classes
class QGridLayout;
class QSplitter;
class QSplitter;
class QTextEdit;
class QListView;
class QListView;

class QSvn : public QSvnDlg
{
    Q_OBJECT

public:
    QSvn( QWidget *parent = 0, const char *name = 0 );
    ~QSvn();

public slots:
    void exitSlot();

    void aboutSlot();
    void aboutQtSlot();

private:
    QGridLayout *qsvnDlgLayout;
    QSplitter *verticalSplitter;
    QSplitter *horizontalSplitter;
    QTextEdit *statusTextEdit;
    QListView *fileListView;
    QListView *workingCopyListView;
};

#endif

