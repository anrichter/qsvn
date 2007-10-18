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
#include "pathproperties.h"
#include "pathpropertiesmodel.h"


PathProperties::PathProperties(QObject *parent, const QString path)
    : QDialog(0)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setupUi(this);
    this->setWindowTitle(QString(tr("Edit Properties for %1")).arg(path));

    m_model = new PathPropertiesModel(path);
    viewPathProperties->setModel(m_model);

    Config::instance()->restoreWidget(this);
    Config::instance()->restoreHeaderView(this, viewPathProperties->header());
}


PathProperties::~PathProperties()
{
    Config::instance()->saveWidget(this);
    Config::instance()->saveHeaderView(this, viewPathProperties->header());
    delete(m_model);
}

void PathProperties::doPathProperties(QWidget *parent, const QString path)
{
    PathProperties *pathProperties = new PathProperties(parent, path);
    pathProperties->show();
    pathProperties->raise();
    pathProperties->activateWindow();
}

#include "pathproperties.moc"
