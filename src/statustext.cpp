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
#include "statustext.h"

//Qt
#include <QtGlobal>
#include <QObject>
#include <QTextEdit>


//make StatusText a singleton
StatusText* StatusText::m_instance = 0;

StatusText* StatusText::instance()
{
    if ( m_instance == 0 )
    {
        m_instance = new StatusText;
    }
    return m_instance;
}

//FileList implementation
StatusText::StatusText( QObject *parent )
        : QObject( parent )
{
    editStatusText = 0;
}

void StatusText::setOutPutWidget( QTextEdit *textEdit )
{
    if ( textEdit )
        editStatusText = textEdit;
}

void StatusText::outputMessage( const QString messageString )
{
    if ( editStatusText )
        editStatusText->append( messageString );
    else
        qDebug( messageString.toAscii() );
}

void StatusText::outputMessage( QStringList messageStringList )
{
    QStringList::const_iterator it;
    for ( it = messageStringList.constBegin(); it != messageStringList.constEnd(); ++it )
        this->outputMessage( *it );
}
