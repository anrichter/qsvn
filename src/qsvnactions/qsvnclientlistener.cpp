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

#include "qsvnactions/qsvnclientlistener.h"
#include "qsvnclientlistener.moc"

#include "svnqt/context_listener.hpp"

#include <QtCore>
#include <QtGui>


QSvnClientListener::QSvnClientListener(QObject *parent)
        : QObject(parent), svn::ContextListener()
{}

QSvnClientListener::~ QSvnClientListener()
{
}

bool QSvnClientListener::contextGetLogin(const QString &realm, QString &username,
                               QString &password, bool &maySave)
{
    return true;
}

void QSvnClientListener::contextNotify(const char *path,
                             svn_wc_notify_action_t action,
                             svn_node_kind_t kind,
                             const char *mime_type,
                             svn_wc_notify_state_t content_state,
                             svn_wc_notify_state_t prop_state,
                             svn_revnum_t revision )
{
}

void QSvnClientListener::contextNotify(const svn_wc_notify_t *action)
{
    contextNotify(action->path, action->action, action->kind, action->mime_type,
                  action->content_state, action->prop_state, action->revision);
}

bool QSvnClientListener::contextCancel()
{
    return false;
}

bool QSvnClientListener::contextGetLogMessage(QString &msg,
                                    const svn::CommitItemList &itemList)
{
    return false;
}

svn::ContextListener::SslServerTrustAnswer
        QSvnClientListener::contextSslServerTrustPrompt(const SslServerTrustData &data,
                                              apr_uint32_t &acceptedFailures)
{
    return svn::ContextListener::SslServerTrustAnswer();
}

bool QSvnClientListener::contextSslClientCertPrompt(QString &certFile)
{
    return false;
}

bool QSvnClientListener::contextSslClientCertPwPrompt(QString &password,
                                            const QString &realm,
                                            bool &maySave)
{
    return false;
}

void QSvnClientListener::contextProgress(long long int current, long long int max)
{}
