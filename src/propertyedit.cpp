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


//QSvn
#include "propertyedit.h"
#include "propertyedit.moc"

//Qt
#include <QDialog>

PropertyEdit::PropertyEdit(QWidget *parent, QString &propertyName, QString &propertyValue)
    : QDialog(parent)
{
    setupUi(this);

    comboPropertyName->addItem("svn:executable");
    comboPropertyName->addItem("svn:mime-type");
    comboPropertyName->addItem("svn:ignore");
    comboPropertyName->addItem("svn:keywords");
    comboPropertyName->addItem("svn:eol-style");
    comboPropertyName->addItem("svn:externals");
    comboPropertyName->addItem("svn:special");

    comboPropertyName->setEditText(propertyName);
    editPropertyValue->setPlainText(propertyValue);
}

PropertyEdit::~ PropertyEdit()
{
}

int PropertyEdit::doPropertyEdit(QWidget *parent, QString &propertyName, QString &propertyValue)
{
    PropertyEdit *edit = new PropertyEdit(parent, propertyName, propertyValue);
    int result = edit->exec();
    if (result == QDialog::Accepted)
    {
        propertyName = edit->comboPropertyName->currentText();
        propertyValue = edit->editPropertyValue->toPlainText();
    }
    return result;
}
