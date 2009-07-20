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
#include "checkout.h"
#include "checkout.moc"
#include "config.h"
#include "login.h"
#include "statustext.h"
#include "qsvnactions/qsvnclientcheckoutaction.h"

//Qt
#include <QtGui>


Checkout::Checkout(QWidget *parent)
        : QDialog(0)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    setupUi(this);
    setWindowIcon(QIcon(":/images/actioncheckout.svg"));

    stackedWidget->setCurrentWidget(pageRepository);
    Config::instance()->restoreWidget(this, pageRepository->objectName());

    editURL->addItems(Config::instance()->getStringList("checkoutURL"));
    editURL->clearEditText();

    editPath->setText(Config::instance()->value(KEY_LASTWC).toString());

    m_selectedURL = "";
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(onDoCheckout()));
    buttonBox->button(QDialogButtonBox::Abort)->setVisible(false);
}

Checkout::~Checkout()
{
    if (stackedWidget->currentWidget() == pageMessages)
        Config::instance()->saveWidget(this, pageMessages->objectName());
    else
        Config::instance()->saveWidget(this, pageRepository->objectName());
}

QString Checkout::url() const
{
    return m_selectedURL;
}

QString Checkout::path() const
{
    QString path = editPath->text();
    while (path.endsWith(QDir::separator()))
        path.chop(1);
    return path;
}

void Checkout::on_buttonPath_clicked()
{
    QString directory =
            QFileDialog::getExistingDirectory(this,
                                              tr("Select a directory for working copy"),
                                              editPath->text());
    if (!directory.isEmpty())
        editPath->setText(QDir::toNativeSeparators(directory));
}

void Checkout::onDoCheckout()
{
    if (editURL->currentText().isEmpty())
    {
        QMessageBox::critical(this, tr("QSvn - Error"), tr("You must specify an URL for checkout!"));
        return;
    }
    if (editPath->text().isEmpty())
    {
        QMessageBox::critical(this, tr("QSvn - Error"), tr("You must specify a directory for checkout!"));
        return;
    }
    QDir dir(editPath->text());
    if (!dir.exists())
    {
        if (QMessageBox::question(this,
                                  tr("QSvn - Question"),
                                  QString(tr("<center>Directoy<br />%1<br />does not exists.<br />Should i create it for you?</center>"))
                                          .arg(editPath->text()),
                                  QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            dir.mkdir(editPath->text());
        }
        else
            return;
    }

    m_selectedURL = editURL->currentText();
    while (m_selectedURL.endsWith(QDir::separator()))
        m_selectedURL.chop(1);

    switchToPageMessages();

    editURL->insertItem(0, m_selectedURL);
    QStringList urlList;
    for (int i = 0; i < editURL->count(); ++i)
        if (!urlList.contains(editURL->itemText(i)))
            urlList << editURL->itemText(i);
    Config::instance()->saveStringList("checkoutURL", urlList);
    Config::instance()->setValue(KEY_LASTWC, editPath->text());

    groupBoxRepository->setEnabled(false);
    buttonBox->button(QDialogButtonBox::Abort)->setVisible(true);
    buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
    buttonBox->button(QDialogButtonBox::Ok)->setVisible(false);

    action = new QSvnClientCheckoutAction(url(), path());
    connect(action, SIGNAL(notify(QString, QString)), this, SLOT(onNotify(QString, QString)));
    connect(action, SIGNAL(finished()), this, SLOT(onCheckoutFinished()));
    connect(action, SIGNAL(finished(QString)), this, SIGNAL(finished(QString)));
    connect(action, SIGNAL(doGetLogin(QString,QString,QString,bool)), this, SLOT(onGetLogin(QString,QString,QString,bool)));
    connect(buttonBox->button(QDialogButtonBox::Abort), SIGNAL(clicked()), action, SLOT(cancelAction()));
    action->start();
}

void Checkout::onNotify(QString action, QString path)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(twMessages);
    item->setText(0, action);
    item->setText(1, path);
    twMessages->addTopLevelItem(item);
    twMessages->scrollToItem(item);
}

void Checkout::onCheckoutFinished()
{
    buttonBox->button(QDialogButtonBox::Ok)->setVisible(true);
    buttonBox->button(QDialogButtonBox::Abort)->setVisible(false);
    disconnect(buttonBox, SIGNAL(accepted()), this, SLOT(onDoCheckout()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

void Checkout::onGetLogin(QString realm, QString username, QString password, bool maySave)
{
    if (Login::doLogin(this, realm, username, password, maySave))
        action->endGetLogin(username, password, maySave);
    else
        action->abortGetLogin();
}

void Checkout::switchToPageMessages()
{
    Config::instance()->saveWidget(this, pageRepository->objectName());
    labelMessages->setText(QString("%1 \n into \n %2").arg(url()).arg(path()));
    stackedWidget->setCurrentWidget(pageMessages);
    Config::instance()->restoreWidget(this, pageMessages->objectName());
}
