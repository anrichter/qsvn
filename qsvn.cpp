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
 *                                                                         *
 *   As a special exception, permission is given to link this program      *
 *   with any edition of Qt, and distribute the resulting executable,      *
 *   without including the source code for Qt in the source distribution.  *
 ***************************************************************************/

//QSvn
#include "qsvn.h"
#include "addworkingcopy.h"
#include "svnclient.h"

//Qt
#include <qobject.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qlistview.h>

#include <qmessagebox.h>
#include <qapplication.h>

QSvn::QSvn( QWidget *parent, const char *name )
        : QSvnDlg( parent, name )
{
    //set dialogs to nil
    addWorkingCopy = 0;
}

QSvn::~QSvn()
{}

void QSvn::exitSlot()
{
    this->close();
}

void QSvn::addWorkingCopySlot()
{
    if ( !addWorkingCopy )
    {
        addWorkingCopy = new AddWorkingCopy( this );
    }

    if ( addWorkingCopy->exec() && SvnClient::Exemplar()->isWorkingCopy( addWorkingCopy->getSelectedDirectory() ) )
    {
        qDebug( "TODO: add workingCopy " + addWorkingCopy->getSelectedDirectory() + " to workingCopyListView" );
    }
    else
    {
        qDebug( SvnClient::Exemplar()->getProcessStderr() );
    }
}

void QSvn::aboutSlot()
{
    QMessageBox::about( this, tr( "About qsvn" ),
                        tr( "This Programm is a simple Subversion Client\n"
                            "Authors: Andreas Richter (ar@oszine.de)" ) );
}

void QSvn::aboutQtSlot()
{
    QMessageBox::aboutQt( this );
}
