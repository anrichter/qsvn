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
#include "checkout.h"
#include "statustext.h"

//Qt
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qmessagebox.h>


Checkout::Checkout( QWidget *parent, const char *name )
        : CheckoutDlg( parent, name )
{}

Checkout::~Checkout()
{}

QString Checkout::getSelectedURL() const
{
    return editURL->text();
}

QString Checkout::getSelectedDirectory() const
{
    return editDirectory->text();
}

void Checkout::selectURLSlot()
{
    StatusText::Exemplar()->outputMessage( QString( "not implemented yet") );
}

void Checkout::selectDirectorySlot()
{
    QString directory = QFileDialog::getExistingDirectory( editDirectory->text(), this, "get", "Select a Directory for Working Copy" );
    if ( directory )
        editDirectory->setText( QDir::convertSeparators( directory ) );
}

void Checkout::buttonOkClickedSlot()
{
    StatusText::Exemplar()->outputMessage( QString( "button ok clicked" ) );
    if ( editURL->text().isEmpty() )
    {
        QMessageBox::critical( this, "qsvn", "your must specified an URL for checkout" );
        return;
    }
    if ( editDirectory->text().isEmpty() )
    {
        QMessageBox::critical( this, "qsvn", "your must specified an Directory for checkout" );
        return;
    }
    QDir dir( editDirectory->text() );
    if ( !dir.exists() )
    {
        if ( QMessageBox::question( this, "qsvn - Question", 
                                    QString( "Directoy %1 does not exist. Should i create this?").arg( editDirectory->text() ),
                                    QMessageBox::Yes,
                                    QMessageBox::No ) == QMessageBox::Yes )
        {
            StatusText::Exemplar()->outputMessage( QString( "Create directory" ) );
        }
    }
    this->accept();
}
