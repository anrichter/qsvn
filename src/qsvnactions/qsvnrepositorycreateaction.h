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

#ifndef QSVNREPOSITORYCREATEACTION_H
#define QSVNREPOSITORYCREATEACTION_H

#include "qsvnactions/qsvnaction.h"

#include "svnqt/repositorylistener.hpp"

class QSvnRepositoryCreateAction : public QSvnAction, public svn::repository::RepositoryListener
{
    public:
        QSvnRepositoryCreateAction(const QString & path, const QString & fstype);

        //Listener
        void sendWarning(const QString&msg);
        void sendError(const QString&msg);
        bool isCanceld();

    protected:
        void run();

    private:
        QString m_path, m_fstype, m_errorString;
};

#endif // QSVNREPOSITORYCREATEACTION_H
