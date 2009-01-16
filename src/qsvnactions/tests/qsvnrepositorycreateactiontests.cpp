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

#include "helper.h"
#include "qsvnactions/qsvnrepositorycreateaction.h"

#include <QtTest/QtTest>
#include <QDir>
#include <QSignalSpy>


class QSvnRepositoryCreateActionTests: public QObject
{
    Q_OBJECT

    private:
        QDir testrepoDir;

    private slots:
        void initTestCase();
        void cleanup();
        void testCreateFSFS();
        void testCreateBDB();
};

void QSvnRepositoryCreateActionTests::initTestCase()
{
    testrepoDir = QDir(QDir::tempPath() + QDir::separator() + "testrepo");
}

void QSvnRepositoryCreateActionTests::cleanup()
{
    Helper::removeFromDisk(testrepoDir.absolutePath());
}

void QSvnRepositoryCreateActionTests::testCreateFSFS()
{
    QVERIFY2(!testrepoDir.exists(), "The test-repository already exists.");

    QSvnRepositoryCreateAction *action = new QSvnRepositoryCreateAction(testrepoDir.absolutePath(), "fsfs");
    QSignalSpy spyFinished(action, SIGNAL(finished()));

    QVERIFY2(!testrepoDir.exists(), "Don't create new repo right after create the action.");

    action->start();
    while (action->isRunning()) {}

    QVERIFY2(testrepoDir.exists(), "Repository doesn't create successfull.");
    QVERIFY2(spyFinished.count() == 1 , "Finished is not emitted.");
    //todo: check the repository filesystem.
}

void QSvnRepositoryCreateActionTests::testCreateBDB()
{
    QVERIFY2(!testrepoDir.exists(), "The test-repository already exists.");
    QSvnRepositoryCreateAction *action = new QSvnRepositoryCreateAction(testrepoDir.absolutePath(), "bdb");
    QVERIFY2(!testrepoDir.exists(), "Don't create new repo right after create the action.");
    action->start();
    while (action->isRunning()) {}
    QVERIFY2(testrepoDir.exists(), "Repository doesn't create successfull.");
    //todo: check the repository filesystem.
}

QTEST_MAIN(QSvnRepositoryCreateActionTests)

#include "qsvnrepositorycreateactiontests.moc"
