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


#ifndef QSVNREPOSITORACTION_H
#define QSVNREPOSITORACTION_H

#include "qsvnaction.h"

#include "svnqt/repositorylistener.hpp"


class QSvnRepositoryAction : public QSvnAction, public svn::repository::RepositoryListener
{
    Q_OBJECT

    public:
        QSvnRepositoryAction(QObject * parent = 0);
        //Listener
        void sendWarning(const QString&msg);
        void sendError(const QString&msg);
        bool isCanceld();
};

#endif // QSVNREPOSITORACTION_H
