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


//QSvn
#include "addworkingcopy.h"

//Qt
#include <QFileDialog>


AddWorkingCopy::AddWorkingCopy( QWidget *parent )
        : QDialog( parent )
{
    setupUi( this );

    QObject::connect( buttonDirectory, SIGNAL( clicked() ), this, SLOT( selectDirectorySlot() ) );
}

QString AddWorkingCopy::getSelectedDirectory() const
{
    return editDirectory->text();
}

void AddWorkingCopy::selectDirectorySlot()
{
    QString directory = QFileDialog::getExistingDirectory( this, tr( "Select a working Directory" ), 
                                                           editDirectory->text(), QFileDialog::ShowDirsOnly );
    if ( !directory.isEmpty() )
        editDirectory->setText( QDir::convertSeparators( directory ) );
}
