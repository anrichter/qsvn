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

#include "helper.h"
#include "qsvnactions/qsvnclientcheckoutaction.h"
#include "qsvnactions/qsvnrepositorycreateaction.h"

#include "svnqt/wc.hpp"

#include <QtTest/QtTest>
#include <QDir>


class QSvnClientCheckoutActionTests: public QObject
{
    Q_OBJECT

    public:
        QSvnClientCheckoutActionTests();

    private:
        QString m_reposPath;
        QDir m_wcDir;

    private slots:
        void initTestCase();
        void cleanupTestCase();
        void cleanup();

        void testCheckoutFile();
        void testCheckoutHTTP();
        void testSignalNotify();
        void testSignalFinished();
        void tetsSignalFinishedNotOnError();
        void testSignalFinishedNotOnEmptyURL();
        void testSignalFinishedNotOnEmptyWC();
        void testCancelAction();
};

QSvnClientCheckoutActionTests::QSvnClientCheckoutActionTests()
{
    m_reposPath = QDir::tempPath() + "/qsvntestrepo";
    m_wcDir = QDir(QDir::tempPath() + QDir::separator() + "qsvntestwc");
}

void QSvnClientCheckoutActionTests::initTestCase()
{
    QSvnRepositoryCreateAction *action =
            new QSvnRepositoryCreateAction(this, m_reposPath, "fsfs");
    action->start();
    while (action->isRunning()) {}
}

void QSvnClientCheckoutActionTests::cleanupTestCase()
{
    Helper::removeFromDisk(m_reposPath);
}

void QSvnClientCheckoutActionTests::cleanup()
{
    Helper::removeFromDisk(m_wcDir.absolutePath());
}

void QSvnClientCheckoutActionTests::testCheckoutFile()
{
    QVERIFY2(!m_wcDir.exists(), "working copy directory exists before checkout");
    QSvnClientCheckoutAction *checkoutAction =
            new QSvnClientCheckoutAction("file:///" + m_reposPath, m_wcDir.absolutePath());
    checkoutAction->start();
    while (checkoutAction->isRunning()) {}
    delete checkoutAction;
    QVERIFY2(m_wcDir.exists(), "working copy directory doesn't exists after checkout");
    QVERIFY2(svn::Wc::checkWc(m_wcDir.absolutePath()), "It's not a valid Working Copy.");
}

void QSvnClientCheckoutActionTests::testCheckoutHTTP()
{
    QVERIFY2(!m_wcDir.exists(), "working copy directory exists before checkout");
    QSvnClientCheckoutAction *action =
            new QSvnClientCheckoutAction("http://www.anrichter.net/svn/qsvn/trunk/scripts", m_wcDir.absolutePath());
    action->start();
    while (action->isRunning()) {}
    delete action;
    QVERIFY2(m_wcDir.exists(), "working copy directory doesn't exists after checkout");
    QVERIFY2(svn::Wc::checkWc(m_wcDir.absolutePath()), "It's not a valid Working Copy.");
}

void QSvnClientCheckoutActionTests::testSignalNotify()
{
    QSvnClientCheckoutAction *checkoutAction =
            new QSvnClientCheckoutAction("file:///" + m_reposPath, m_wcDir.absolutePath());
    QSignalSpy spyProgress(checkoutAction, SIGNAL(notify(QString, QString)));
    checkoutAction->start();
    while (checkoutAction->isRunning()) {}
    QVERIFY2(spyProgress.count() != 0 , "Notify never emitted.");
}

void QSvnClientCheckoutActionTests::testSignalFinished()
{
    QSvnClientCheckoutAction *checkoutAction =
            new QSvnClientCheckoutAction("file:///" + m_reposPath, m_wcDir.absolutePath());
    QSignalSpy spyFinished(checkoutAction, SIGNAL(finished(QString)));
    checkoutAction->start();
    while (checkoutAction->isRunning()) {}
    QVERIFY2(spyFinished.count() == 1, "signal finished(QString) was not emitted.");
}

void QSvnClientCheckoutActionTests::tetsSignalFinishedNotOnError()
{
    QSvnClientCheckoutAction *checkoutAction =
            new QSvnClientCheckoutAction("file:///" + m_reposPath + "error", m_wcDir.absolutePath());
    QSignalSpy spyFinished(checkoutAction, SIGNAL(finished(QString)));
    checkoutAction->start();
    while (checkoutAction->isRunning()) {}
    QVERIFY2(spyFinished.count() == 0, "Don't call signal finished(QString) when an error occurred.");
}

void QSvnClientCheckoutActionTests::testSignalFinishedNotOnEmptyURL()
{
    QSvnClientCheckoutAction *checkoutAction =
            new QSvnClientCheckoutAction("", m_wcDir.absolutePath());
    QSignalSpy spyFinished(checkoutAction, SIGNAL(finished(QString)));
    checkoutAction->start();
    while (checkoutAction->isRunning()) {}
    QVERIFY2(spyFinished.count() == 0, "Don't call signal finished(QString) when the URL is empty.");
}

void QSvnClientCheckoutActionTests::testSignalFinishedNotOnEmptyWC()
{
    QSvnClientCheckoutAction *checkoutAction =
            new QSvnClientCheckoutAction("file:///" + m_reposPath + "error", "");
    QSignalSpy spyFinished(checkoutAction, SIGNAL(finished(QString)));
    checkoutAction->start();
    while (checkoutAction->isRunning()) {}
    QVERIFY2(spyFinished.count() == 0, "Don't call signal finished(QString) when the WC-dir is empty.");
}

void QSvnClientCheckoutActionTests::testCancelAction()
{
    QSvnClientCheckoutAction *checkoutAction =
            new QSvnClientCheckoutAction("file:///" + m_reposPath, m_wcDir.absolutePath());
    QSignalSpy spyTerminated(checkoutAction, SIGNAL(terminated()));
    checkoutAction->start();
    checkoutAction->cancelAction();
    while (checkoutAction->isRunning()) {}
    QVERIFY2(spyTerminated.count() != 0, "cancelAction don't terminate the QSvnAction.");
}


QTEST_MAIN(QSvnClientCheckoutActionTests)
#include "qsvnclientcheckoutactiontests.moc"
