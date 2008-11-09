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

#ifndef PATHPROPERTIES_H
#define PATHPROPERTIES_H

//QSvn
#include "ui_pathproperties.h"

class PathPropertiesModel;

//Qt
#include <QDialog>

class QPushButton;


class PathProperties : public QDialog, public Ui::PathProperties
{
        Q_OBJECT

    public:
        PathProperties(QWidget *parent, const QString path);
        ~PathProperties();

        static void doPathProperties(QWidget *parent, const QString path);

    public slots:
        void accept();

    private:
        PathPropertiesModel *m_model;
        QPushButton *addButton, *deleteButton, *editButton;  //todo: rename to button*

    private slots:
        void addButtonClicked();
        void deleteButtonClicked();
        void editButtonClicked();
        void updateButtons();
};

#endif
