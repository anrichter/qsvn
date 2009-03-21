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
}

void QSvnClientAction::contextNotify(const svn_wc_notify_t *action)
{
    QString actionString;
    switch (action->action)
    {
        /** Adding a path to revision control. */
    case svn_wc_notify_add:
        actionString = tr("Add");
        break;

        /** Copying a versioned path. */
    case svn_wc_notify_copy:
        actionString = tr("Copy");
        break;

        /** Deleting a versioned path. */
    case svn_wc_notify_delete:
        actionString = tr("Delete");
        break;

        /** Restoring a missing path from the pristine text-base. */
    case svn_wc_notify_restore:
        actionString = tr("Restore");
        break;

        /** Reverting a modified path. */
    case svn_wc_notify_revert:
        actionString = tr("Revert");
        break;

        /** A revert operation has failed. */
    case svn_wc_notify_failed_revert:
        actionString = tr("Revert failed");
        break;

        /** Resolving a conflict. */
    case svn_wc_notify_resolved:
        actionString = tr("Resolved");
        break;

        /** Skipping a path. */
    case svn_wc_notify_skip:
        actionString = tr("Skip");
        break;

        /** Got a delete in an update. */
    case svn_wc_notify_update_delete:
        actionString = tr("Deleted");
        break;

        /** Got an add in an update. */
    case svn_wc_notify_update_add:
        actionString = tr("Added");
        break;

        /** Got any other action in an update. */
    case svn_wc_notify_update_update:
        actionString = tr("Updated");
        break;

        /** The last notification in an update (including updates of externals). */
    case svn_wc_notify_update_completed:
        actionString = tr("Completed");
        break;

        /** Updating an external module. */
    case svn_wc_notify_update_external:
        actionString = tr("Update external");
        break;

        /** The last notification in a status (including status on externals). */
    case svn_wc_notify_status_completed:
        actionString = tr("Status completed");
        break;

        /** Running status on an external module. */
    case svn_wc_notify_status_external:
        actionString = tr("Status external");
        break;

        /** Committing a modification. */
    case svn_wc_notify_commit_modified:
        actionString = tr("Commit modified");
        break;

        /** Committing an addition. */
    case svn_wc_notify_commit_added:
        actionString = tr("Commit added");
        break;

        /** Committing a deletion. */
    case svn_wc_notify_commit_deleted:
        actionString = tr("Commit deleted");
        break;

        /** Committing a replacement. */
    case svn_wc_notify_commit_replaced:
        actionString = tr("Commit replaced");
        break;

        /** Transmitting post-fix text-delta data for a file. */
    case svn_wc_notify_commit_postfix_txdelta:
        actionString = tr("Commit postfix txdelta");
        break;

        /** Processed a single revision's blame. */
    case svn_wc_notify_blame_revision:
        actionString = tr("Blame revision");
        break;

        /** Locking a path. @since New in 1.2. */
    case svn_wc_notify_locked:
        actionString = tr("Locked");
        break;

        /** Unlocking a path. @since New in 1.2. */
    case svn_wc_notify_unlocked:
        actionString = tr("Unlocked");
        break;

        /** Failed to lock a path. @since New in 1.2. */
    case svn_wc_notify_failed_lock:
        actionString = tr("Failed lock");
        break;

        /** Failed to unlock a path. @since New in 1.2. */
    case svn_wc_notify_failed_unlock:
        actionString = tr("Failed unlock");
        break;

        /** Tried adding a path that already exists. @since New in 1.5. */
    case svn_wc_notify_exists:
        actionString = tr("Exists");
        break;

        /** Changelist name set. @since New in 1.5. */
    case svn_wc_notify_changelist_set:
        actionString = tr("Changelist set");
        break;

        /** Changelist name cleared. @since New in 1.5. */
    case svn_wc_notify_changelist_clear:
        actionString = tr("Changelist clear");
        break;

        /** Warn user that a path has moved from one changelist to another.
      @since New in 1.5. */
    case svn_wc_notify_changelist_moved:
        actionString = tr("Changelist moved");
        break;

        /** A merge operation (to path) has begun.  See @c merge_range in
      @c svn_wc_notify_t.  @since New in 1.5   */
    case svn_wc_notify_merge_begin:
        actionString = tr("Merge begin");
        break;

        /** A merge operation (to path) from a foreign repository has begun.
      See @c merge_range in @c svn_wc_notify_t. @since New in 1.5. */
    case svn_wc_notify_foreign_merge_begin:
        actionString = tr("Foreign merge begin");
        break;

        /** Replace notification. @since New in 1.5. */
    case svn_wc_notify_update_replace:
        actionString = tr("Update replace");
        break;
    }

    emit notify(actionString, QString(action->path));
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
