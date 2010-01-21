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


#include "qsvnactions/qsvnrepositorycreateaction.h"

#include "svnqt/repoparameter.hpp"
#include "svnqt/repository.hpp"
#include "svnqt/repositorylistener.hpp"


QSvnRepositoryCreateAction::QSvnRepositoryCreateAction(const QString & path, const QString & fstype)
        : QSvnRepositoryAction()
{
    createRepoParameter.path(path).fstype(fstype);
}

QSvnRepositoryCreateAction::QSvnRepositoryCreateAction(QObject * parent, const QString & path, const QString & fstype)
        : QSvnRepositoryAction(parent)
{
    createRepoParameter.path(path).fstype(fstype);
}

void QSvnRepositoryCreateAction::run()
{
    svn::repository::Repository repository(this);
    try
    {
        repository.CreateOpen(createRepoParameter);
    }
    catch (svn::ClientException e)
    {
        exit(-1);
    }
}
