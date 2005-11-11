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
#include "checkout.h"
#include "config.h"
#include "statustext.h"

//Qt
#include <QtGui>


Checkout::Checkout( QWidget *parent )
        : QDialog( parent )
{
    setupUi( this );

    QObject::connect( buttonURL, SIGNAL( clicked() ), this, SLOT( selectURLSlot() ) );
    QObject::connect( buttonPath, SIGNAL( clicked() ), this, SLOT( selectPathSlot() ) );
    QObject::connect( buttonOk, SIGNAL( clicked() ), this, SLOT( buttonOkClickedSlot() ) );

    editURL->addItems( Config::instance()->getStringList( "checkoutURL" ) );
    editURL->clearEditText();
}

QString Checkout::url() const
{
    return editURL->currentText();
}

QString Checkout::path() const
{
    return editPath->text();
}

void Checkout::selectURLSlot()
{
    StatusText::instance()->outputMessage( QString( "not implemented yet") );
}

void Checkout::selectPathSlot()
{
    QString directory = QFileDialog::getExistingDirectory( this, "Select a directory for working copy", editPath->text() );
    if ( !directory.isEmpty() )
        editPath->setText( QDir::convertSeparators( directory ) );
}

void Checkout::buttonOkClickedSlot()
{
    if ( editURL->currentText().isEmpty() )
    {
        QMessageBox::critical( this, "qsvn - Error", "You must specify an URL for checkout!" );
        return;
    }
    if ( editPath->text().isEmpty() )
    {
        QMessageBox::critical( this, "qsvn - Error", "You must specify a directory for checkout!" );
        return;
    }
    QDir dir( editPath->text() );
    if ( !dir.exists() )
    {
        if ( QMessageBox::question( this, "qsvn - Question",
                                    QString( "<center>Directoy<br />%1<br />does not exist.<br />Should i create this?</center>").arg( editPath->text() ),
                                    QMessageBox::Yes,
                                    QMessageBox::No ) == QMessageBox::Yes )
        {
            dir.mkdir( editPath->text() );
        }
        else
            return;
    }

    editURL->insertItem( 0, editURL->currentText() );
    QStringList list;
    for ( int i = 0; i < editURL->count(); ++i )
        list << editURL->itemText( i );
    Config::instance()->saveStringList( "checkoutURL", list );
    this->accept();
}
