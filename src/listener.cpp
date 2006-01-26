/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2006 Andreas Richter <ar@oszine.de>                *
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

//SvnCpp
#include "svncpp/context_listener.hpp"

//Qt
#include "QtCore"


Listener::Listener( )
{}

Listener::~ Listener( )
{}

bool Listener::contextGetLogin( const QString &realm, QString &username, QString &password, bool &maySave)
{
    Login login;
    login.setRealm( realm );
    login.setUsername( username );
    login.setPassword( password );
    login.setMaySave( maySave );

    if ( login.exec() )
    {
        username = login.username();
        password = login.password();
        maySave = login.maySave();
        return true;
    }
    else
        return false;
}

void Listener::contextNotify( const char * path, svn_wc_notify_action_t action, svn_node_kind_t kind, const char * mime_type, svn_wc_notify_state_t content_state, svn_wc_notify_state_t prop_state, svn_revnum_t revision )
{}

#if (SVN_VER_MAJOR >= 1) && (SVN_VER_MINOR >= 2)
void Listener::contextNotify( const svn_wc_notify_t *action )
{
    if ( !action )
        return;

    //action to string
    QString notifyString;
    switch ( action->action )
    {
    case svn_wc_notify_add:
        notifyString = QString( tr( "add" ) );
        break;
    case svn_wc_notify_copy:
        notifyString = QString( tr( "copy" ) );
        break;
    case svn_wc_notify_delete:
        notifyString = QString( tr( "delete" ) );
        break;
    case svn_wc_notify_restore:
        notifyString = QString( tr( "restore" ) );
        break;
    case svn_wc_notify_revert:
        notifyString = QString( tr( "revert" ) );
        break;
    case svn_wc_notify_failed_revert:
        notifyString = QString( tr( "failed revert" ) );
        break;
    case svn_wc_notify_resolved:
        notifyString = QString( tr( "resolved" ) );
        break;
    case svn_wc_notify_skip:
        notifyString = QString( tr( "skip" ) );
        break;
    case svn_wc_notify_update_delete:
        notifyString = QString( tr( "update delete" ) );
        break;
    case svn_wc_notify_update_add:
        notifyString = QString( tr( "update add" ) );
        break;
    case svn_wc_notify_update_update:
        notifyString = QString( tr( "update" ) );
        break;
    case svn_wc_notify_update_completed:
        notifyString = QString( tr( "update completed" ) );
        break;
    case svn_wc_notify_update_external:
        notifyString = QString( tr( "update external" ) );
        break;
    case svn_wc_notify_status_completed:
        notifyString = QString( tr( "status completed" ) );
        break;
    case svn_wc_notify_status_external:
        notifyString = QString( tr( "status external" ) );
        break;
    case svn_wc_notify_commit_modified:
        notifyString = QString( tr( "commit modified" ) );
        break;
    case svn_wc_notify_commit_added:
        notifyString = QString( tr( "commit added" ) );
        break;
    case svn_wc_notify_commit_deleted:
        notifyString = QString( tr( "commit deleted" ) );
        break;
    case svn_wc_notify_commit_replaced:
        notifyString = QString( tr( "commit related" ) );
        break;
    case svn_wc_notify_commit_postfix_txdelta:
        notifyString = QString( tr( "postfix txdelta" ) );
        break;
    case svn_wc_notify_blame_revision:
        notifyString = QString( tr( "blame revision" ) );
        break;
    case svn_wc_notify_locked:
        notifyString = QString( tr( "locked" ) );
        break;
    case svn_wc_notify_unlocked:
        notifyString = QString( tr( "unlocked" ) );
        break;
    case svn_wc_notify_failed_lock:
        notifyString = QString( tr( "failed lock" ) );
        break;
    case svn_wc_notify_failed_unlock:
        notifyString = QString( tr( "failed unlock" ) );
        break;
    }

    notifyString = notifyString + " " + action->path;
    if ( action->revision > -1 )
        notifyString = QString( tr( "%1 Revision %2" ) ).arg( notifyString ).arg ( action->revision );
    StatusText::instance()->outputMessage( notifyString );
}
#endif

bool Listener::contextCancel( )
{
    return false;
}

bool Listener::contextGetLogMessage( QString &msg )
{
    return false;
}

svn::ContextListener::SslServerTrustAnswer Listener::contextSslServerTrustPrompt( const SslServerTrustData & data, apr_uint32_t & acceptedFailures )
{
    return DONT_ACCEPT;
}

bool Listener::contextSslClientCertPrompt( QString & certFile )
{
    return false;
}

bool Listener::contextSslClientCertPwPrompt( QString &password, const QString &realm, bool &maySave  )
{
    return false;
}
