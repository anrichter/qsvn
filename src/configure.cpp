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
#include "config.h"
#include "configure.h"
#include "configure.moc"
#include "ui_configure.h"

//Qt
#include <QtGui>


Configure::Configure(QWidget *parent, Qt::WFlags flags)
        : QDialog(parent, flags)
{
    setupUi(this);
    comboBoxIconsize->addItem(tr("Small (16x16)"), 16);
    comboBoxIconsize->addItem(tr("Medium (22x22)"), 22);
    comboBoxIconsize->addItem(tr("Big (32x32)"), 32);
    comboBoxIconsize->addItem(tr("Huge (46x46"), 46);

    Config::instance()->restoreWidget(this);

    editDiffViewer->setText(Config::instance()->value(KEY_DIFFVIEWER).toString());
    checkBoxShowLogAfterUpdate->setChecked(Config::instance()->value(KEY_SHOWLOGAFTERUPDATE).toBool());
    checkBoxCheckEmptyLogMessage->setChecked(Config::instance()->value(KEY_CHECKEMPTYLOGMESSAGE).toBool());
    checkBoxShowTextInToolbar->setChecked(Config::instance()->value(KEY_SHOWTEXTINTOOLBAR).toBool());
    comboBoxIconsize->setCurrentIndex(comboBoxIconsize->findData(Config::instance()->value(KEY_ICONSIZE, DEFAULT_ICONSIZE).toInt()));
}

Configure::~Configure()
{
    Config::instance()->saveWidget(this);
}

void Configure::accept()
{
    Config::instance()->setValue(KEY_DIFFVIEWER, editDiffViewer->text());
    Config::instance()->setValue(KEY_SHOWLOGAFTERUPDATE, checkBoxShowLogAfterUpdate->checkState());
    Config::instance()->setValue(KEY_CHECKEMPTYLOGMESSAGE, checkBoxCheckEmptyLogMessage->checkState());
    Config::instance()->setValue(KEY_SHOWTEXTINTOOLBAR, checkBoxShowTextInToolbar->checkState());
    Config::instance()->setValue(KEY_ICONSIZE, comboBoxIconsize->itemData(comboBoxIconsize->currentIndex()));
    QDialog::accept();
}

void Configure::on_buttonSelectDiffViewer_clicked()
{
    QString diffviewer = QFileDialog::getOpenFileName(this,
            tr("Select a Diff Viewer"),
            editDiffViewer->text(),
            "");
    if (!diffviewer.isNull())
        editDiffViewer->setText(QDir::convertSeparators(diffviewer));
}
