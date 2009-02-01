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
#include "qsvnactions/qsvnclientcheckoutaction.moc"

#include "svnqt/context_listener.hpp"


QSvnClientCheckoutAction::QSvnClientCheckoutAction(const QString & url, const QString & path)
{
    m_url = url;
    m_path = path;
}

QSvnClientCheckoutAction::~ QSvnClientCheckoutAction()
{
    delete m_context;
    emit progress(100);
}

bool QSvnClientCheckoutAction::contextGetLogin(const QString & realm,
                                               QString & username,
                                               QString & password,
                                               bool & maySave)
{
    return false;
}

void QSvnClientCheckoutAction::contextNotify(const char *path,
                                             svn_wc_notify_action_t action,
                                             svn_node_kind_t kind,
                                             const char *mime_type,
                                             svn_wc_notify_state_t content_state,
                                             svn_wc_notify_state_t prop_state,
                                             svn_revnum_t revision)
{}

void QSvnClientCheckoutAction::contextNotify(const svn_wc_notify_t *action)
{}

bool QSvnClientCheckoutAction::contextCancel()
{
    return false;
}

bool QSvnClientCheckoutAction::contextGetLogMessage(QString &msg, const svn::CommitItemList&)
{
    return false;
}

svn::ContextListener::SslServerTrustAnswer QSvnClientCheckoutAction::contextSslServerTrustPrompt
        (const SslServerTrustData &data,
         apr_uint32_t &acceptedFailures)
{
    return svn::ContextListener::SslServerTrustAnswer();
}

bool QSvnClientCheckoutAction::contextSslClientCertPrompt(QString &certFile)
{
    return false;
}

bool QSvnClientCheckoutAction::contextSslClientCertPwPrompt(QString &password,
                                                            const QString &realm,
                                                            bool &maySave)
{
    return false;
}

bool QSvnClientCheckoutAction::contextLoadSslClientCertPw(QString&, const QString&)
{
    return false;
}

bool QSvnClientCheckoutAction::contextGetSavedLogin(const QString&, QString&, QString&)
{
    return false;
}

bool QSvnClientCheckoutAction::contextGetCachedLogin(const QString & realm, QString & username, QString & password)
{
    return false;
}

void QSvnClientCheckoutAction::contextProgress(long long int current, long long int max)
{
    int _percent =  current * 100 / max;
    emit progress(_percent);
}

void QSvnClientCheckoutAction::run()
{
    emit progress(0);

    if (m_url.isEmpty() || m_path.isEmpty())
        exit(-1);

    m_context = new svn::Context();
    m_client = svn::Client::getobject(m_context, 0);
    m_context->setListener(this);

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
