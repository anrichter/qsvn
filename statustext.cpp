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
#include "statustext.h"

//Qt
#include <qtextedit.h>

//make StatusText a singleton
StatusText* StatusText::_exemplar = 0;

StatusText* StatusText::Exemplar()
{
    if ( _exemplar == 0 )
    {
        _exemplar = new StatusText;
    }
    return _exemplar;
}

//FileList implementation
StatusText::StatusText(QObject *parent, const char *name)
        : QObject(parent, name)
{
    editStatusText = new QTextEdit( 0, "editStatusText" );
    editStatusText->setWordWrap( QTextEdit::WidgetWidth );
    editStatusText->setReadOnly( TRUE );
}

StatusText::~StatusText()
{}

QWidget *StatusText::getWidget()
{
    return editStatusText;
}

void StatusText::outputMessage( const QString messageString )
{
    if ( editStatusText )
        editStatusText->append( messageString );
    else
        qDebug( messageString );
}

