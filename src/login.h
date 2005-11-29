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
 ***************************************************************************/


#ifndef LOGIN_H
#define LOGIN_H

//QSvn
#include "ui_login.h"

//Qt
#include <QtGui>


class Login : public QDialog, public Ui::Login
{
    Q_OBJECT

public:
    Login( QWidget *parent = 0 );

    void setRealm( const QString &realm );
    void setUsername( QString &username );
    void setPassword( QString &password );
    void setMaySave( bool &maySave );

    QString username();
    QString password();
    bool maySave();

public slots:

private:

protected slots:
};

#endif
