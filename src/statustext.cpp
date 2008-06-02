/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2008 Andreas Richter <ar@anrichter.net>                 *
 *                                                                              *
 *   This program is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License Version 2             *
 *   as published by the Free Software Foundation.                              *
 *                                                                              *
 *   This program is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 *   GNU General Public License for more details.                               *
 *                                                                              *
 *   You should have received a copy of the GNU General Public License          *
 *   along with this program; if not, write to the                              *
 *   Free Software Foundation, Inc.,                                            *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                  *
 *                                                                              *
 *******************************************************************************/

//QSvn
#include "statustext.h"

//Qt
#include <QtGui>


//make StatusText a singleton
StatusText* StatusText::m_instance = 0;

StatusText* StatusText::instance()
{
    if (m_instance == 0)
    {
        m_instance = new StatusText;
    }
    return m_instance;
}

//StatusText implementation
StatusText::StatusText(QObject *parent)
        : QObject(parent)
{
    editStatusText = 0;
}

void StatusText::setOutPutWidget(QTextEdit *textEdit)
{
    if (textEdit)
        editStatusText = textEdit;
}

void StatusText::outString(const QString &s)
{
    if (editStatusText)
    {
        editStatusText->append(s);
        qApp->processEvents();
    }
    else
        qDebug(s.toAscii());
}
