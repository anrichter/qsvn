/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2010 Andreas Richter <ar@anrichter.net>                 *
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

#ifndef STATUSTEXT_H
#define STATUSTEXT_H

//Qt
#include <QObject>

class QTextEdit;


/**
This class handle the status text output

@author Andreas Richter
*/

class StatusText : public QObject
{
        Q_OBJECT

    public:
        inline static void setOut(QTextEdit *textEdit)
        {
            StatusText::instance()->setOutPutWidget(textEdit);
        }

        inline static void out(const QString &s)
        {
            StatusText::instance()->outString(s);
        }

    private:
        StatusText(QObject *parent = 0);

        static StatusText *m_instance;
        static StatusText* instance();

        QTextEdit *editStatusText;
        void outString(const QString &s);
        void setOutPutWidget(QTextEdit *textEdit);
};

#endif
