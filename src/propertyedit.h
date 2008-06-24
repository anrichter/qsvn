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

#ifndef HEADER_H
#define HEADER_H

//QSvn
#include "ui_propertyedit.h"

//Qt
#include <QDialog>

class PropertyEdit : public QDialog, public Ui::PropertyEdit
{
    Q_OBJECT
    public:
        PropertyEdit(QWidget *parent, QString &propertyName, QString &propertyValue);
        ~PropertyEdit();

        static int doPropertyEdit(QWidget *parent, QString &propertyName, QString &propertyValue);
};



#endif
