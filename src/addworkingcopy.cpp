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

 
//QSvn
#include "addworkingcopy.h"
#include "addworkingcopydlg.h"

//Qt
#include <qdir.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>


AddWorkingCopy::AddWorkingCopy( QWidget *parent, const char *name )
        : AddWorkingCopyDlg( parent, name )
{
    QObject::connect( buttonDirectory, SIGNAL( clicked() ), this, SLOT( selectDirectorySlot() ) );
}

AddWorkingCopy::~AddWorkingCopy()
{}

QString AddWorkingCopy::getSelectedDirectory() const
{
    return editDirectory->text();
}

void AddWorkingCopy::selectDirectorySlot()
{
    QString directory = QFileDialog::getExistingDirectory( editDirectory->text(), this, "get", "Select a working Directory" );
    if ( directory )
        editDirectory->setText( QDir::convertSeparators( directory ) );
}
