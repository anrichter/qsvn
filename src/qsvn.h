/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2005 Andreas Richter <ar@oszine.de>                *
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
 *                                                                         *
 *   As a special exception, permission is given to link this program      *
 *   with any edition of Qt, and distribute the resulting executable,      *
 *   without including the source code for Qt in the source distribution.  *
***************************************************************************/

#ifndef QSVN_H
#define QSVN_H

//QSvn
#include "qsvndlg.h"


class QSvn : public QSvnDlg
{
    Q_OBJECT

public:
    QSvn( QWidget *parent = 0, const char *name = 0 );
    ~QSvn();

public slots:

protected slots:
    //File
    void exitSlot();
    
    //WorkingCopy
    void addWorkingCopySlot();
    void removeWorkingCopySlot();
    void checkoutSlot();
    
    //Modify
    void updateSlot();
    void commitSlot();
    void addSlot();
    void removeSlot();
    void revertSlot();
    
    //Query
    void diffSlot();
    
    //Settings
    void configureQSvnSlot();
    
    //Help
    void helpContentsSlot();
    void helpIndexSlot();
    void aboutSlot();
    void aboutQtSlot();
    
    // @brief prepare informations for and call a svn command during SvnClient::doSvnCommand
    void svnCommand( int svnCommandType, bool withFileSelector = FALSE );
};

#endif
