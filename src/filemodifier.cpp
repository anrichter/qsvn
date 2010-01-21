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
#include "filemodifier.h"
#include "filemodifier.moc"
#include "svnclient.h"

//Qt
#include <QFileDialog>


FileModifier::FileModifier(QWidget *parent, QString path, SvnClient::SvnAction svnAction)
        : QDialog(parent)
{
    setupUi(this);

    m_svnAction = svnAction;
    m_srcFilePath = path;
    QFileInfo _fileInfo(m_srcFilePath);
    m_srcFile = _fileInfo.fileName();
    m_srcPath = _fileInfo.path();

    switch (m_svnAction)
    {
        case SvnClient::SvnRename:
            setWindowTitle(tr("Rename"));
            labelFromFile->setText(m_srcFile);
            editToFile->setText(m_srcFile);
            buttonPath->setVisible(false);
            break;
        case SvnClient::SvnMove:
            setWindowTitle(tr("Move"));
            labelFromFile->setText(m_srcPath);
            editToFile->setText(m_srcPath);
            buttonPath->setVisible(true);
            break;
        case SvnClient::SvnCopy:
            setWindowTitle(tr("Copy"));
            labelFromFile->setText(m_srcPath);
            editToFile->setText(m_srcPath);
            buttonPath->setVisible(true);
            break;
        case SvnClient::SvnMkDir:
            setWindowTitle(tr("Make Directory"));
            labelFromFile->setVisible(false);
            labelTo->setVisible(false);
            buttonPath->setVisible(false);
            break;
    }
}

FileModifier::~FileModifier()
{}

void FileModifier::accept()
{
    switch (m_svnAction)
    {
        case SvnClient::SvnRename:
            SvnClient::instance()->move(m_srcFilePath, m_srcPath + "/" + editToFile->text(), true);
            break;
        case SvnClient::SvnMove:
            SvnClient::instance()->move(m_srcFilePath, editToFile->text(), true);
            break;
        case SvnClient::SvnCopy:
            SvnClient::instance()->copy(m_srcFilePath, editToFile->text());
            break;
        case SvnClient::SvnMkDir:
            SvnClient::instance()->mkdir(m_srcFilePath + "/" + editToFile->text());
            break;
    }
    QDialog::accept();
}

void FileModifier::on_buttonPath_clicked()
{
    QString directory =
            QFileDialog::getExistingDirectory(this,
                                              tr("Select a directory"),
                                                      editToFile->text());
    if (!directory.isEmpty())
        editToFile->setText(QDir::fromNativeSeparators(directory));
}
