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
#include "filemodifier.h"
#include "filemodifier.moc"
#include "svnclient.h"


FileModifier::FileModifier(QWidget *parent, QString path, SvnClient::SvnAction svnAction)
        : QDialog(parent)
{
    setupUi(this);

    m_svnAction = svnAction;
    m_srcPath = path;

    switch (m_svnAction)
    {
        case SvnClient::SvnRename:
            setWindowTitle(tr("Rename"));
            break;
        case SvnClient::SvnMove:
            setWindowTitle(tr("Move"));
            break;
        case SvnClient::SvnCopy:
            setWindowTitle(tr("Copy"));
            break;
        case SvnClient::SvnMkDir:
            setWindowTitle(tr("Make Directory"));
            labelFromFile->setVisible(false);
            labelTo->setVisible(false);
            break;
    }
    labelFromFile->setText(m_srcPath);
    editToFile->setText(m_srcPath);
}

FileModifier::~FileModifier()
{}

void FileModifier::accept()
{
    switch (m_svnAction)
    {
        case SvnClient::SvnRename:
            SvnClient::instance()->move(m_srcPath, editToFile->text(), true);
            break;
        case SvnClient::SvnMove:
            SvnClient::instance()->move(m_srcPath, editToFile->text(), true);
            break;
        case SvnClient::SvnCopy:
            SvnClient::instance()->copy(m_srcPath, editToFile->text());
            break;
        case SvnClient::SvnMkDir:
            SvnClient::instance()->mkdir(editToFile->text());
            break;
    }
    QDialog::accept();
}
