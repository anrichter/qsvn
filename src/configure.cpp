/***************************************************************************
 *   This file is part of QSvn Project http://ar.oszine.de/projects/qsvn   *
 *   Copyright (c) 2004-2007 Andreas Richter <ar@oszine.de>                *
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
#include "config.h"
#include "configure.h"
#include "ui_configure.h"

//Qt
#include <QtGui>


Configure::Configure( QWidget *parent, Qt::WFlags flags )
        : QDialog( parent, flags )
{
    setupUi( this );
    Config::instance()->restoreWidget( this );

    editDiffViewer->setText( Config::instance()->value( KEY_DIFFVIEWER ).toString() );
    if ( Config::instance()->value( KEY_SHOWLOGAFTERUPDATE ).toBool() )
        checkBoxShowLogAfterUpdate->setCheckState( Qt::Checked );
    else
        checkBoxShowLogAfterUpdate->setCheckState( Qt::Unchecked );
    if ( Config::instance()->value( KEY_CHECKEMPTYLOGMESSAGE ).toBool() )
        checkBoxCheckEmptyLogMessage->setCheckState( Qt::Checked );
    else
        checkBoxCheckEmptyLogMessage->setCheckState( Qt::Unchecked );
}

Configure::~ Configure( )
{
    Config::instance()->saveWidget( this );
}

void Configure::accept()
{
    Config::instance()->setValue( KEY_DIFFVIEWER, editDiffViewer->text() );
    if ( checkBoxShowLogAfterUpdate->checkState() == Qt::Checked )
        Config::instance()->setValue( KEY_SHOWLOGAFTERUPDATE, true );
    else
        Config::instance()->setValue( KEY_SHOWLOGAFTERUPDATE, false );
    if ( checkBoxCheckEmptyLogMessage->checkState() == Qt::Checked )
        Config::instance()->setValue( KEY_CHECKEMPTYLOGMESSAGE, true );
    else
        Config::instance()->setValue( KEY_CHECKEMPTYLOGMESSAGE, false );
    QDialog::accept();
}

void Configure::on_buttonSelectDiffViewer_clicked()
{
    QString diffviewer = QFileDialog::getOpenFileName( this, tr( "Select a Diff Viewer" ), editDiffViewer->text(), "" );
    if ( !diffviewer.isNull() )
        editDiffViewer->setText( QDir::convertSeparators( diffviewer ) );
}

#include "configure.moc"
