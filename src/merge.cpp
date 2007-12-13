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
#include "merge.h"
#include "svnclient.h"

//SvnCpp
#include "svnqt/revision.hpp"

//Qt
#include <QtGui>


void Merge::doMerge(const QString fromURL, const svn::Revision fromRevision,
                    const QString toURL, const svn::Revision toRevision)
{
    Merge *merge = new Merge();
    merge->setFromUrl(fromURL);
    merge->setFromRevision(fromRevision);
    merge->setToUrl(toURL);
    merge->setToRevision(toRevision);
    merge->show();
    merge->raise();
    merge->activateWindow();
}

Merge::Merge()
    : QDialog(0) //don't set parent here! It is always a top-level window
{
    setupUi(this);
    buttonDryRun = new QPushButton(tr("Dry run"));
    buttonDryRun->setDefault(false);
    buttonBox->addButton(buttonDryRun, QDialogButtonBox::ActionRole);
    connect(buttonDryRun, SIGNAL(clicked()), this, SLOT(on_buttonDryRun_clicked()));

    editWcPath->setText(Config::instance()->value(KEY_LASTMERGEWC).toString());
}

Merge::~Merge()
{
}

void Merge::setFromUrl(const QString fromURL)
{
    editFromUrl->setText(fromURL);
}

void Merge::setFromRevision(const svn::Revision fromRevision)
{
    editFromRevision->setText(fromRevision);
}

void Merge::setToUrl(const QString toURL)
{
    editToUrl->setText(toURL);
}

void Merge::setToRevision(const svn::Revision toRevision)
{
    editToRevision->setText(toRevision);
}

void Merge::on_buttonWcPath_clicked( )
{
    QString path =
            QFileDialog::getExistingDirectory(this,
                                              tr("Merge differences into Working Copy"),
                                              editWcPath->text());
    if (!path.isEmpty())
        editWcPath->setText(QDir::toNativeSeparators(path));

}

void Merge::accept()
{
    SvnClient::instance()->merge(editFromUrl->text(),
                                 svn::Revision(editFromRevision->text().toUInt()),
                                 editToUrl->text(),
                                 svn::Revision(editToRevision->text().toUInt()),
                                 editWcPath->text(),
                                 true, true, false, false);

    Config::instance()->setValue(KEY_LASTMERGEWC, editWcPath->text());
    QDialog::accept();
}

void Merge::on_buttonDryRun_clicked( )
{
    SvnClient::instance()->merge(editFromUrl->text(),
                                 svn::Revision(editFromRevision->text().toUInt()),
                                 editToUrl->text(),
                                 svn::Revision(editToRevision->text().toUInt()),
                                 editWcPath->text(),
                                 true, true, false, true);
}

#include "merge.moc"
