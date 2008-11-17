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

//QSvn
#include "listener.h"
#include "listener.moc"
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

    QString _path = QDir::toNativeSeparators(path);
    switch (action)
    {
        case svn_wc_notify_add:
            if (mime_type && (svn_mime_type_is_binary(mime_type)))
                notifyString = tr("A    (bin) %1").arg(_path);
            else
                notifyString = tr("A    %1").arg(_path);
            break;
        case svn_wc_notify_copy:
            break;
        case svn_wc_notify_delete:
            notifyString = tr("D    %1").arg(_path);
            break;
        case svn_wc_notify_restore:
            notifyString = tr("Restored '%1'").arg(_path);
            break;
        case svn_wc_notify_revert:
            notifyString = tr("Reverted '%1'").arg(_path);
            break;
        case svn_wc_notify_failed_revert:
            notifyString = tr("Failed to revert '%1' -- try updating instead.").arg(_path);
            break;
        case svn_wc_notify_resolved:
            notifyString = tr("Resolved conflicted state of '%1'").arg(_path);
            break;
        case svn_wc_notify_skip:
            if (content_state == svn_wc_notify_state_missing)
                notifyString = tr("Skipped missing target: '%1'").arg(_path);
            else
                notifyString = tr("Skipped '%1'").arg(_path);
            break;
        case svn_wc_notify_update_add:
            if (content_state == svn_wc_notify_state_conflicted)
                notifyString = tr("C    %1").arg(_path);
            else
                notifyString = tr("A    %1").arg(_path);
            break;
        case svn_wc_notify_update_delete:
            notifyString = tr("D    %1").arg(_path);
            break;
        case svn_wc_notify_update_update:
            /* If this is an inoperative dir change, do no notification.
            An inoperative dir change is when a directory gets closed
            without any props having been changed. */
            if (! ((kind == svn_node_dir)
                    && ((prop_state == svn_wc_notify_state_inapplicable)
                    || (prop_state == svn_wc_notify_state_unknown)
                    || (prop_state == svn_wc_notify_state_unchanged))))
            {
                notifyString = "";
                //First position is for file
                if (kind == svn_node_file) {
                    if (content_state == svn_wc_notify_state_changed)
                        notifyString.append("U");
                    else if (content_state == svn_wc_notify_state_merged)
                        notifyString.append("G");
                    else if (content_state == svn_wc_notify_state_conflicted)
                        notifyString.append("C");
                    else
                        notifyString.append(" ");
                }
                //Second position is for property
                if (prop_state == svn_wc_notify_state_changed)
                    notifyString.append("U");
                else if (prop_state == svn_wc_notify_state_merged)
                    notifyString.append("G");
                else if (prop_state == svn_wc_notify_state_conflicted)
                    notifyString.append("C");
                else
                    notifyString.append(" ");

                if (!notifyString.trimmed().isEmpty())
                    notifyString.append("   ").append(_path);
                else
                    notifyString.clear();
            }
            break;
        case svn_wc_notify_update_completed:
            //todo: switch between update,checkout, export (internal, external)
            if (SVN_IS_VALID_REVNUM(revision))
                notifyString = tr("Finished at revision %1.").arg(revision);
            else
                notifyString = tr("Finished.");
            break;
        case svn_wc_notify_update_external:
            notifyString = tr("Fetching external item into '%1'").arg(_path);
            break;
        case svn_wc_notify_status_completed:
            if (SVN_IS_VALID_REVNUM(revision))
                notifyString = tr("Status against revision: %1").arg(revision);
            break;
        case svn_wc_notify_status_external:
            notifyString = tr("Performing status on external item at %1").arg(_path);
            break;
        case svn_wc_notify_commit_modified:
            notifyString = tr("Sending        %1").arg(_path);
            break;
        case svn_wc_notify_commit_added:
            if (mime_type && svn_mime_type_is_binary(mime_type))
            {
                notifyString = tr("Adding  (bin)  %1").arg(_path);
            } else {
                notifyString = tr("Adding         %1").arg(_path);
            }
            break;
        case svn_wc_notify_commit_deleted:
            notifyString = tr("Deleting       %1").arg(_path);
            break;
        case svn_wc_notify_commit_replaced:
            notifyString = tr("Replacing      %1").arg(_path);
            break;
        case svn_wc_notify_commit_postfix_txdelta:
            notifyString = tr("Transmitting file %1").arg(_path);
            break;
        case svn_wc_notify_blame_revision:
            break;
        case svn_wc_notify_locked:
            notifyString = tr("'%1' locked.").arg(_path);
            break;
        case svn_wc_notify_unlocked:
            notifyString = tr("'%1' unlocked.").arg(_path);
            break;
        case svn_wc_notify_failed_lock:
            notifyString = tr("Failed to lock '%1'").arg(_path);
            break;
        case svn_wc_notify_failed_unlock:
            notifyString = tr("Failed to unlock '%1'").arg(_path);
            break;
    }

    if (!notifyString.isEmpty())
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
