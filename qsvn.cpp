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

//QSvn
#include "qsvn.h"

//Qt
#include <qobject.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qlistview.h>

#include <qmessagebox.h>


QSvn::QSvn( QWidget *parent, const char *name )
  : QSvnDlg( parent, name )
{
    //create layout 
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );
	qsvnDlgLayout = new QGridLayout( centralWidget(), 1, 1, 0, 0, "qsvn_Dlg_Layout" );
	
	verticalSplitter = new QSplitter( QSplitter::Vertical, centralWidget() );
	verticalSplitter->setHandleWidth( 2 );

	qsvnDlgLayout->addWidget( verticalSplitter, 0, 0 );

	horizontalSplitter = new QSplitter( QSplitter::Horizontal, verticalSplitter );
	horizontalSplitter->setHandleWidth( 2 );

	workingCopyListView = new QListView( horizontalSplitter, "workingCopyListView" );
    workingCopyListView->addColumn( tr( "Working Copy" ) );

	fileListView = new QListView( horizontalSplitter, "fileListView" );
    fileListView->addColumn( tr( "Filename" ) );
    fileListView->addColumn( tr( "Extension" ) );
    fileListView->addColumn( tr( "State" ) );

	statusTextEdit = new QTextEdit( verticalSplitter, "statusTextEdit" );

	verticalSplitter->setOpaqueResize( TRUE );
}

QSvn::~QSvn()
{
}

void QSvn::exitSlot()
{
    this->close();
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
