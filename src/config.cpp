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
#include "qsvn.h"

//Qt
#include <QtGui>


//make Config a singleton
Config* Config::m_instance = 0;

Config* Config::instance()
{
    if (m_instance == 0)
    {
        m_instance = new Config;
    }
    return m_instance;
}

//Config implementation
Config::Config(QObject *parent)
        : QObject(parent)
{
    m_settings = new QSettings( "QSvn", "QSvn", this );

    m_tempDir = QDir::tempPath() + QDir::separator() + "qsvn" + QDir::separator();
    m_tempDir = QDir::toNativeSeparators(m_tempDir);
}

void Config::removeTempDir()
{
    QDir tempDir(m_tempDir);
    QStringListIterator it(tempDir.entryList());
    while (it.hasNext())
        tempDir.remove(it.next());

    tempDir.rmdir(m_tempDir);
}

void Config::setIniFile(const QString iniFile)
{
    if (m_settings)
        delete(m_settings);

    m_settings = new QSettings(iniFile, QSettings::IniFormat, this);
    m_settings->sync();
}

void Config::saveMainWindow(const QSvn *aQSvn)
{
    if (aQSvn)
    {
        saveWidget(aQSvn);

        saveSplitter(aQSvn, aQSvn->splitterVertical);
        saveSplitter(aQSvn, aQSvn->splitterHorizontal);
    }
}

void Config::restoreMainWindow(QSvn *aQSvn)
{
    if (aQSvn)
    {
        restoreWidget(aQSvn);

        //restore settings from splitterVertical
        restoreSplitter(aQSvn, aQSvn->splitterVertical);
        restoreSplitter(aQSvn, aQSvn->splitterHorizontal);
    }
}

void Config::saveWidget(const QWidget *widget, const QString prefix)
{
    QString key = "widget" + prefix + widget->objectName();

    m_settings->sync();
    m_settings->remove(key);
    m_settings->setValue(key + "/geometry", widget->saveGeometry());
    m_settings->sync();
}

void Config::restoreWidget(QWidget *widget, const QString prefix)
{
    QString key = "widget" + prefix + widget->objectName();

    m_settings->sync();
    widget->restoreGeometry(m_settings->value(key + "/geometry").toByteArray());
}

void Config::saveSplitter(const QObject *parent, const QSplitter *splitter)
{
    QString key = parent->objectName() + "_" + splitter->objectName();

    m_settings->sync();
    m_settings->remove(key);
    m_settings->beginWriteArray(key, splitter->sizes().count());
    for (int i = 0; i < splitter->sizes().count(); ++i)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue("value", splitter->sizes().at(i));
    }
    m_settings->endArray();
    m_settings->sync();
}

void Config::restoreSplitter(const QObject *parent, QSplitter *splitter)
{
    QString key = parent->objectName() + "_" + splitter->objectName();

    m_settings->sync();
    int size = m_settings->beginReadArray(key);
    if (size <= 0)
    {
        m_settings->endArray();
        return;
    }

    QList<int> list = splitter->sizes();
    for (int i = 0; i < size; i++)
    {
        m_settings->setArrayIndex(i);
        list.replace(i, m_settings->value("value").toInt());
    }
    m_settings->endArray();
    m_settings->sync();

    splitter->setSizes(list);
}

void Config::saveStringList(const QString &prefix, const QStringList &stringList)
{
    if (prefix.isEmpty())
        return;

    m_settings->sync();
    m_settings->remove(prefix);
    m_settings->beginWriteArray(prefix, stringList.count());
    for (int i = 0; i < stringList.count(); ++i)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue("entry" , stringList.at(i));
    }
    m_settings->endArray();
    m_settings->sync();
}

QStringList Config::getStringList(const QString &prefix)
{
    QStringList stringList;

    m_settings->sync();
    int size = m_settings->beginReadArray(prefix);

    for (int i = 0; i < size; ++i)
    {
        m_settings->setArrayIndex(i);
        stringList.append(m_settings->value("entry").toString());
    }
    m_settings->endArray();
    m_settings->sync();

    return stringList;
}

void Config::setValue(const QString &key, const QVariant &value)
{
    m_settings->sync();
    m_settings->setValue(key, value);
    m_settings->sync();
}

QVariant Config::value(const QString &key)
{
    m_settings->sync();
    return m_settings->value(key, defaultValue(key));
    m_settings->sync();
}

void Config::saveHeaderView(const QObject *parent, const QHeaderView *headerView)
{
    QString key = parent->objectName() + "_" + headerView->parent()->objectName();

    m_settings->sync();
    m_settings->remove(key);
    m_settings->beginWriteArray(key, headerView->count());
    for (int i = 0; i < headerView->count(); ++i)
    {
        m_settings->setArrayIndex(i);
        m_settings->setValue("value", headerView->sectionSize(i));
    }
    m_settings->endArray();
    key = key + "_sort";
    m_settings->setValue(key + "/section", headerView->sortIndicatorSection());
    m_settings->setValue(key + "/order",   QVariant(headerView->sortIndicatorOrder()));
    m_settings->sync();
}

void Config::restoreHeaderView(const QObject *parent, QHeaderView *headerView)
{
    QString key = parent->objectName() + "_" + headerView->parent()->objectName();

    m_settings->sync();
    m_settings->beginReadArray(key);
    for (int i = 0; i < headerView->count(); i++)
    {
        m_settings->setArrayIndex(i);
        headerView->resizeSection(i, m_settings->value("value", headerView->sectionSize(i)).toInt());
    }
    m_settings->endArray();

    key = key + "_sort";
    headerView->setSortIndicator(m_settings->value(key + "/section", headerView->sortIndicatorSection()).toInt(),
                                 Qt::SortOrder(m_settings->value(key + "/order", headerView->sortIndicatorSection()).toInt()));
    m_settings->sync();
}

QString Config::tempDir()
{
    QDir dir(m_tempDir);
    if (!dir.exists())
        dir.mkpath(m_tempDir);
    return m_tempDir;
}

QVariant Config::defaultValue(const QString &key)
{
    if (key == KEY_SHOWLOGAFTERUPDATE)
        return true;
    else if (key == KEY_CHECKEMPTYLOGMESSAGE)
        return true;
    else if (key == KEY_LASTWC)
        return QDir::toNativeSeparators(QDir::homePath());
    else
        return QVariant();
}
