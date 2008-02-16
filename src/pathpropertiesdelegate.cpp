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
#include "pathpropertiesdelegate.h"

//Qt
#include <QComboBox>
#include <QItemDelegate>


PathPropertiesDelegate::PathPropertiesDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget * PathPropertiesDelegate::createEditor(QWidget *parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(true);
    editor->addItem("svn:executable");
    editor->addItem("svn:mime-type");
    editor->addItem("svn:ignore");
    editor->addItem("svn:keywords");
    editor->addItem("svn:eol-style");
    editor->addItem("svn:externals");
    editor->addItem("svn:special");

    return editor;
}

void PathPropertiesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setEditText(index.model()->data(index, Qt::DisplayRole).toString());
}

void PathPropertiesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                          const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void PathPropertiesDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);
}


#include "pathpropertiesdelegate.moc"
