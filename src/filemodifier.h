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

#ifndef FILEMODIFIER_H
#define FILEMODIFIER_H

//QSvn
#include "ui_filemodifier.h"
#include "svnclient.h"

//Qt
#include <QDialog>


class FileModifier : public QDialog, public Ui::FileModifier
{
        Q_OBJECT

    public:
        FileModifier(QWidget *parent, QString path, SvnClient::SvnAction svnAction);
        ~FileModifier();

    public slots:
        void accept();

    private:
        SvnClient::SvnAction m_svnAction;
        QString m_srcPath;
};

#endif
