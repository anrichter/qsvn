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

#include "qsvnclientaction.h"
#include "qsvnclientaction.moc"
#include "svnqt/context_listener.hpp"

QSvnClientAction::QSvnClientAction(QObject * parent)
        : QSvnAction(parent)
{
    m_context = new svn::Context();
    m_client = svn::Client::getobject(m_context, 0);
    m_context->setListener(this);
    qRegisterMetaType<svn_wc_notify_t>("svn_wc_notify_t");
}

QSvnClientAction::~QSvnClientAction()
{
    delete m_context;
}

bool QSvnClientAction::contextGetLogin(const QString & realm,
                                       QString & username,
                                       QString & password,
                                       bool & maySave)
{
    return false;
}

void QSvnClientAction::contextNotify(const char *path,
                                     svn_wc_notify_action_t action,
                                     svn_node_kind_t kind,
                                     const char *mime_type,
                                     svn_wc_notify_state_t content_state,
                                     svn_wc_notify_state_t prop_state,
                                     svn_revnum_t revision)
{
    svn_wc_notify_t _notify;
    _notify.path = path;
    _notify.action = action;
    _notify.kind = kind;
    _notify.mime_type = mime_type;
    _notify.content_state = content_state;
    _notify.prop_state = prop_state;
    _notify.revision = revision;
    emit notify(_notify);
}

void QSvnClientAction::contextNotify(const svn_wc_notify_t *action)
{
    svn_wc_notify_t _notify;
    _notify.path = action->path;
    _notify.action = action->action;
    _notify.kind = action->kind;
    _notify.mime_type = action->mime_type;
    _notify.content_state = action->content_state;
    _notify.prop_state = action->prop_state;
    _notify.revision = action->revision;
    emit notify(_notify);
}

bool QSvnClientAction::contextCancel()
{
    return false;
}

bool QSvnClientAction::contextGetLogMessage(QString &msg, const svn::CommitItemList&)
{
    return false;
}

svn::ContextListener::SslServerTrustAnswer QSvnClientAction::contextSslServerTrustPrompt
        (const SslServerTrustData &data,
         apr_uint32_t &acceptedFailures)
{
    return svn::ContextListener::SslServerTrustAnswer();
}

bool QSvnClientAction::contextSslClientCertPrompt(QString &certFile)
{
    return false;
}

bool QSvnClientAction::contextSslClientCertPwPrompt(QString &password,
                                                            const QString &realm,
                                                            bool &maySave)
{
    return false;
}

bool QSvnClientAction::contextLoadSslClientCertPw(QString&, const QString&)
{
    return false;
}

bool QSvnClientAction::contextGetSavedLogin(const QString&, QString&, QString&)
{
    return false;
}

bool QSvnClientAction::contextGetCachedLogin(const QString & realm, QString & username, QString & password)
{
    return false;
}

void QSvnClientAction::contextProgress(long long int current, long long int max)
{}
