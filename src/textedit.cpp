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
#include "config.h"
#include "textedit.h"


//Qt
#include <QDialog>


bool TextEdit::edit(QWidget * parent, const QString title, QString & text)
{
    TextEdit *dlg = new TextEdit(parent, title, text);
    bool result = false;
    if (dlg->exec())
    {
        text = dlg->textEdit->toPlainText();
        result = true;
    }
    delete dlg;
    return result;
}

TextEdit::TextEdit(QWidget * parent, const QString title, const QString text)
    : QDialog(parent)
{
    setupUi(this);
    setWindowTitle(title);
    Config::instance()->restoreWidget(this, title);
    textEdit->setPlainText(text);
}

TextEdit::~ TextEdit()
{
    Config::instance()->saveWidget(this, windowTitle());
}
