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


#include "qsvnactions/qsvnclientcheckoutaction.h"

QSvnClientCheckoutAction::QSvnClientCheckoutAction(const QString & url, const QString & path)
{
    m_url = url;
    m_path = path;
}

QSvnClientCheckoutAction::~ QSvnClientCheckoutAction()
{
    delete m_listener;
    delete m_context;
}

void QSvnClientCheckoutAction::run()
{
    if (m_url.isEmpty() || m_path.isEmpty())
        exit(-1);

    m_context = new svn::Context();
    m_client = svn::Client::getobject(m_context, 0);
    m_listener = new QSvnClientListener();
    m_context->setListener(m_listener);

    try
    {
        m_client->checkout(m_url, m_path, svn::Revision::HEAD);
    }
    catch (svn::ClientException e)
    {
        m_errorString = e.msg();
        exit(-1);
    }
}
