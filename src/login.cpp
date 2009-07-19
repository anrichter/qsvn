/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2009 Andreas Richter <ar@anrichter.net>                 *
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
#include "login.h"
#include "login.moc"

//Qt
#include <QtGui>


Login::Login(QWidget *parent)
        : QDialog(parent)
{
    setupUi(this);
}

bool Login::doLogin(QWidget *parent, QString &realm, QString &username, QString &password, bool &maySave)
{
    Login login;
    login.setRealm(realm);
    login.setUsername(username);
    login.setPassword(password);
    login.setMaySave(maySave);

    if (login.exec())
    {
        username = login.username();
        password = login.password();
        maySave = login.maySave() == true;
        return true;
    }
    else
        return false;
}

void Login::setRealm(const QString &realm)
{
    labelRealm->setText(realm);
}

void Login::setUsername(QString &username)
{
    editUsername->setText(username);
}

void Login::setPassword(QString &password)
{
    editPassword->setText(password);
}

void Login::setMaySave(bool &maySave)
{
    if (maySave)
        checkSaveAuth->setCheckState(Qt::Checked);
    else
        checkSaveAuth->setCheckState(Qt::Unchecked);
}

QString Login::username()
{
    return editUsername->text();
}

QString Login::password()
{
    return editPassword->text();
}

bool Login::maySave()
{
    if (checkSaveAuth->checkState() == Qt::Checked)
        return true;
    else
        return false;
}
