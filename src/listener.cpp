/***************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
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

//QSvn
#include "listener.h"
#include "login.h"
#include "statustext.h"
#include "sslservertrust.h"

//SvnCpp
#include "svnqt/context_listener.hpp"

//Qt
#include <QtCore>
#include <QtGui>


Listener::Listener(QObject *parent)
        : QObject(parent), svn::ContextListener()
{
    m_cancel = false;
    m_verbose = true;
}

Listener::~ Listener( )
{}

bool Listener::contextGetLogin(const QString &realm, QString &username,
                               QString &password, bool &maySave)
{
    Login login;
    login.setRealm(realm);
    login.setUsername(username);
    login.setPassword(password);
    login.setMaySave(maySave);

    if (login.exec())
    {
        username = login.username();
        password = login.password();
        maySave = login.maySave() == true;
        return true;
    }
    else
        return false;
}

void Listener::contextNotify(const char *path,
                             svn_wc_notify_action_t action,
                             svn_node_kind_t kind,
                             const char *mime_type,
                             svn_wc_notify_state_t content_state,
                             svn_wc_notify_state_t prop_state,
                             svn_revnum_t revision )
{
    if (!m_verbose)
        return;

    //action to string
    QString notifyString;
    switch (action)
    {
        case svn_wc_notify_add:
            notifyString = QString(tr("add"));
            break;
        case svn_wc_notify_copy:
            notifyString = QString(tr("copy"));
            break;
        case svn_wc_notify_delete:
            notifyString = QString(tr("delete"));
            break;
        case svn_wc_notify_restore:
            notifyString = QString(tr("restore"));
            break;
        case svn_wc_notify_revert:
            notifyString = QString(tr("revert"));
            break;
        case svn_wc_notify_failed_revert:
            notifyString = QString(tr("failed revert"));
            break;
        case svn_wc_notify_resolved:
            notifyString = QString(tr("resolved"));
            break;
        case svn_wc_notify_skip:
            notifyString = QString(tr("skip"));
            break;
        case svn_wc_notify_update_delete:
            notifyString = QString(tr("update delete"));
            break;
        case svn_wc_notify_update_add:
            notifyString = QString(tr("update add"));
            break;
        case svn_wc_notify_update_update:
            notifyString = QString(tr("update"));
            break;
        case svn_wc_notify_update_completed:
            notifyString = QString(tr("update completed"));
            break;
        case svn_wc_notify_update_external:
            notifyString = QString(tr("update external"));
            break;
        case svn_wc_notify_status_completed:
            notifyString = QString(tr("status completed"));
            break;
        case svn_wc_notify_status_external:
            notifyString = QString(tr("status external"));
            break;
        case svn_wc_notify_commit_modified:
            notifyString = QString(tr("commit modified"));
            break;
        case svn_wc_notify_commit_added:
            notifyString = QString(tr("commit added"));
            break;
        case svn_wc_notify_commit_deleted:
            notifyString = QString(tr("commit deleted"));
            break;
        case svn_wc_notify_commit_replaced:
            notifyString = QString(tr("commit related"));
            break;
        case svn_wc_notify_commit_postfix_txdelta:
            notifyString = QString(tr("postfix txdelta"));
            break;
        case svn_wc_notify_blame_revision:
            notifyString = QString(tr("blame revision"));
            break;
        case svn_wc_notify_locked:
            notifyString = QString(tr("locked"));
            break;
        case svn_wc_notify_unlocked:
            notifyString = QString(tr("unlocked"));
            break;
        case svn_wc_notify_failed_lock:
            notifyString = QString(tr("failed lock"));
            break;
        case svn_wc_notify_failed_unlock:
            notifyString = QString(tr("failed unlock"));
            break;
    }

    notifyString = notifyString + " " + QDir::toNativeSeparators(QDir::cleanPath(path));
    if (revision > -1)
        notifyString = QString(tr("%1 Revision %2"))
                .arg(notifyString)
                .arg(revision);
    StatusText::out(notifyString);
}

void Listener::contextNotify(const svn_wc_notify_t *action)
{
    if (!action)
        return;

    contextNotify(action->path, action->action, action->kind, action->mime_type,
                  action->content_state, action->prop_state, action->revision);

    qApp->processEvents();
}

bool Listener::contextCancel()
{
    if (m_cancel)
    {
        m_cancel = false;
        return true;
    }
    else
        return false;
}

bool Listener::contextGetLogMessage(QString &msg,
                                    const svn::CommitItemList &itemList)
{
    return false;
}

svn::ContextListener::SslServerTrustAnswer
        Listener::contextSslServerTrustPrompt(const SslServerTrustData &data,
                                              apr_uint32_t &acceptedFailures)
{
    return SslServerTrust::getSslServerTrustAnswer(data);
}

bool Listener::contextSslClientCertPrompt(QString &certFile)
{
    return false;
}

bool Listener::contextSslClientCertPwPrompt(QString &password,
                                            const QString &realm,
                                            bool &maySave)
{
    return false;
}

void Listener::setCancel(bool cancel)
{
    m_cancel = cancel;
}

void Listener::setVerbose(bool verbose)
{
    m_verbose = verbose;
}

void Listener::contextProgress(long long int current, long long int max)
{}
