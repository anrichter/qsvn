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
#include "pathproperties.h"
#include "pathpropertiesmodel.h"
#include "propertyedit.h"
#include "statustext.h"
#include "svnclient.h"

#include "svnqt/status.hpp"

//Qt
#include <QtGui>


PathProperties::PathProperties(QWidget *parent, const QString path)
    : QDialog(0)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setupUi(this);

    addButton = new QPushButton(this);
    addButton->setText(tr("Add"));
    this->buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));

    editButton = new QPushButton(this);
    editButton->setText(tr("Edit"));
    this->buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);
    connect(editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));

    deleteButton = new QPushButton(this);
    deleteButton->setText(tr("Delete"));
    this->buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));

    this->setWindowTitle(QString(tr("Edit Properties for %1")).arg(path));

    m_model = new PathPropertiesModel(path);
    viewPathProperties->setModel(m_model);
    connect(viewPathProperties->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateButtons()));
    connect(viewPathProperties, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(editButtonClicked()));

    Config::instance()->restoreWidget(this);
    Config::instance()->restoreHeaderView(this, viewPathProperties->header());

    updateButtons();
}


PathProperties::~PathProperties()
{
    Config::instance()->saveWidget(this);
    Config::instance()->saveHeaderView(this, viewPathProperties->header());
    delete(m_model);
}

void PathProperties::doPathProperties(QWidget *parent, const QString path)
{
    svn::StatusPtr status = SvnClient::instance()->singleStatus(path);
    if (status->isVersioned())
    {
        PathProperties *pathProperties = new PathProperties(parent, path);
        pathProperties->show();
        pathProperties->raise();
        pathProperties->activateWindow();
    }
    else
    {
        StatusText::out(QString("Path %1 is not versioned").arg(path));
    }
}

void PathProperties::addButtonClicked()
{
    QString _propName, _propValue;
    if (PropertyEdit::doPropertyEdit(this, _propName, _propValue) == QDialog::Accepted)
    {
        m_model->addProperty(_propName, _propValue);
        updateButtons();
    }
}

void PathProperties::accept()
{
    m_model->writeProperties();
    QDialog::accept();
}

void PathProperties::deleteButtonClicked()
{
    QItemSelectionModel *selection = viewPathProperties->selectionModel();
    m_model->deleteProperty(*selection);
    updateButtons();
}

void PathProperties::editButtonClicked()
{
    if (viewPathProperties->selectionModel()->hasSelection())
    {
        QString _propName, _propValue;
        _propName = m_model->getPropertyName(viewPathProperties->selectionModel()->currentIndex().row());
        _propValue = m_model->getPropertyValue(viewPathProperties->selectionModel()->currentIndex().row());
        if (PropertyEdit::doPropertyEdit(this, _propName, _propValue) == QDialog::Accepted)
            m_model->setProperty(_propName, _propValue);
    }
}

void PathProperties::updateButtons()
{
    editButton->setEnabled(viewPathProperties->selectionModel()->hasSelection());
    deleteButton->setEnabled(viewPathProperties->selectionModel()->hasSelection());
}

#include "pathproperties.moc"
