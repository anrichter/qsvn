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

#ifndef MERGE_H
#define MERGE_H

//QSvn
#include "svnclient.h"
#include "ui_merge.h"

//Qt
#include <QDialog>

class Merge : public QDialog, public Ui::Merge
{
    Q_OBJECT

    public:
        static void doMerge(const QString fromURL, const svn::Revision fromRevision,
                            const QString toURL, const svn::Revision toRevision);

        void setFromUrl(const QString fromURL);
        void setFromRevision(const svn::Revision fromRevision);
        void setToUrl(const QString fromURL);
        void setToRevision(const svn::Revision fromRevision);

    private:
        Merge();
        ~Merge();

};

#endif
