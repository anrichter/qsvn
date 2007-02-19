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
#include "qsvn.h"

//Qt
#include <QApplication>


int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(qsvn);

    QApplication app(argc, argv);
    
    //check command line parameter
    foreach(QString arg, qApp->arguments())
    {
        if (arg.contains("--inifile", Qt::CaseSensitive))
        {
            QStringList list = arg.split("=");
            if ((list.count() == 2) && 
                (!list.at(1).isEmpty()))
            {
                Config::instance()->setIniFile(list.at(1));
            }
        }
    }

    QSvn qsvn;
    qsvn.show();

    return app.exec();
}
