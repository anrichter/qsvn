/***************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2007 Andreas Richter <ar@anrichter.net>                *
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
#include "wcmodel.h"

//SvnCpp
#include "svnqt/wc.hpp"

//Qt
#include <QtGui>


WcModel::WcModel(QObject *parent)
        : QStandardItemModel(parent)
{
    setHorizontalHeaderLabels(QStringList("Working Copy"));
    loadWcList();
}

WcModel::~WcModel()
{
    saveWcList();
}

bool WcModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return true;
    else {
        QStandardItem *_item = itemFromIndex(parent);
        if (!_item->data(PopulatedRole).toBool())
            populate(_item);
        return _item->rowCount();
    }
}

void WcModel::populate(QStandardItem * parent) const
{
    parent->removeRows(0, parent->rowCount());
    foreach (QString _dir, QDir(parent->data(PathRole).toString()).entryList(QDir::AllDirs).filter(QRegExp("^[^.]")))
        insertDir(_dir, parent, parent->rowCount());
    parent->setData(true, PopulatedRole);
}

void WcModel::insertWc(QString dir)
{
    dir = QDir::toNativeSeparators(QDir::cleanPath(dir));
    int row = 0;
    for (int i = 0; i < invisibleRootItem()->rowCount(); i++)
    {
        if (dir > getPath(invisibleRootItem()->child(i)->index()))
            row = i + 1;
    }
    insertDir(dir, invisibleRootItem(), row);
}

void WcModel::removeWc(QString dir)
{
    foreach(QStandardItem* _item, findItems(dir))
    {
        removeRow(_item->row(), _item->index().parent());
    }
}

QString WcModel::getPath(const QModelIndex &index) const
{
    return itemFromIndex(index)->data(PathRole).toString();
}

void WcModel::insertDir(QString dir, QStandardItem * parent, int row) const
{
    QStandardItem *item = new QStandardItem();

    item->setText(QDir::toNativeSeparators(QDir::cleanPath(dir)));

    //complete dir in data(PathRole) to full path for subdirectories of root-wc-items
    if (parent != invisibleRootItem())
        dir = parent->data(PathRole).toString() + QDir::separator() + dir;
    item->setData(QDir::toNativeSeparators(dir), PathRole);

    if (svn::Wc::checkWc(dir.toLocal8Bit()))
        item->setIcon(QIcon(":/images/folder.png"));
    else
        item->setIcon(QIcon(":/images/unknownfolder.png"));

    parent->insertRow(row, item);
}

void WcModel::saveWcList()
{
    QStringList wcList;

    for (int i = 0; i < invisibleRootItem()->rowCount(); i++)
        wcList << invisibleRootItem()->child(i)->data(PathRole).toString();

    Config::instance()->saveStringList("workingCopies", wcList);
}

void WcModel::loadWcList()
{
    QStringList wcList = Config::instance()->getStringList("workingCopies");
    wcList.sort();

    foreach (QString wc, wcList)
        insertDir(wc, invisibleRootItem(), invisibleRootItem()->rowCount());
}

void WcModel::doCollapse(const QModelIndex & index)
{
    itemFromIndex(index)->setData(false, PopulatedRole);
}


#include "wcmodel.moc"
