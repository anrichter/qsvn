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
    svnContext = new svn::Context();
    svnClient = svn::Client::getobject(svnContext, 0);
    svnContext->setListener(this);
    inExternal = false;
    isActionCanceled = false;
    sslServerTrustData = 0;
}

QSvnClientAction::~QSvnClientAction()
{
    if (sslServerTrustData)
        delete sslServerTrustData;
    delete svnContext;
}

bool QSvnClientAction::contextGetLogin(const QString & realm,
                                       QString & username,
                                       QString & password,
                                       bool & maySave)
{
    startEmit();
    emit doGetLogin(realm, username, password, maySave);
    while (emitIsRunning)
        sleep(1);
    if (emitIsAborted)
        return false;
    else
    {
        username = login_username;
        password = login_password;
        maySave = login_maySave;
        return true;
    }
}

void QSvnClientAction::startEmit()
{
    emitIsAborted = false;
    emitIsRunning = true;
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
        if (inExternal)
        {
            actionString = tr("Checked out external at revision %1").arg(action->revision);
            inExternal = false;
        }
        else
            actionString = tr("Checked out at revision %1").arg(action->revision);
        break;

        /** Updating an external module. */
    case svn_wc_notify_update_external:
        inExternal = true;
        actionString = tr("Fetching external item into");
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
    return isActionCanceled;
}

svn::ContextListener::SslServerTrustAnswer QSvnClientAction::contextSslServerTrustPrompt
        (const SslServerTrustData &data,
         apr_uint32_t &acceptedFailures)
{
    startEmit();
    sslServerTrustData = new svn::ContextListener::SslServerTrustData(data);
    emit doGetSslServerTrustPrompt();
    while (emitIsRunning)
        sleep(1);
    if (emitIsAborted)
        return svn::ContextListener::SslServerTrustAnswer();
    else
    {
        return sslprompt_answer;
    }
}

void QSvnClientAction::cancelAction()
{
    isActionCanceled = true;
    terminate();
}

void QSvnClientAction::endGetLogin(QString username, QString password, bool maySave)
{
    login_username = username;
    login_password = password;
    login_maySave = maySave;
    finishEmit();
}

void QSvnClientAction::finishEmit()
{
    emitIsAborted = false;
    emitIsRunning = false;
}

void QSvnClientAction::endGetSslServerTrustPrompt(SslServerTrustAnswer answer)
{
    sslprompt_answer = answer;
    finishEmit();
}

void QSvnClientAction::abortEmit()
{
    emitIsAborted = true;
    emitIsRunning = false;
}

svn::ContextListener::SslServerTrustData QSvnClientAction::getSslServerTrustData()
{
    if (sslServerTrustData)
        return *sslServerTrustData;
    else
        return 0;
}

