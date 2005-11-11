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
 *   As a special exception, permission is given to link this program      *
 *   with any edition of Qt, and distribute the resulting executable,      *
 *   without including the source code for Qt in the source distribution.  *
 ***************************************************************************/


//QSvn
#include "config.h"
#include "configure.h"
#include "ui_configure.h"

//Qt
#include <QtGui>


Configure::Configure( QWidget *parent, Qt::WFlags flags )
        : QDialog( parent, flags )
{
    setupUi( this );

    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( buttonOkClickSlot() ) );
    connect( buttonSelectDiffViewer, SIGNAL( clicked() ), this, SLOT( buttonSelectDiffViewerClickSlot() ) );

    editDiffViewer->setText( Config::instance()->getDiffViewer() );
}

void Configure::buttonOkClickSlot()
{
    Config::instance()->setDiffViewer( editDiffViewer->text() );
}

void Configure::buttonSelectDiffViewerClickSlot()
{
    QString diffviewer = QFileDialog::getOpenFileName( this, "Select a Diff Viewer", editDiffViewer->text(), "" );
    if ( !diffviewer.isNull() )
        editDiffViewer->setText( QDir::convertSeparators( diffviewer ) );
}
