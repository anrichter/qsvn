/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2005 Andreas Richter <ar@oszine.de>                *
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
 *   As a special exception, permission is given to link this program      *
 *   with any edition of Qt, and distribute the resulting executable,      *
 *   without including the source code for Qt in the source distribution.  *
 ***************************************************************************/

//QSvn
#include "listener.h"

//SvnCpp
#include "svncpp/context_listener.hpp"


Listener::Listener( )
{
}

Listener::~ Listener( )
{
}

bool Listener::contextGetLogin( const std::string & realm, std::string & username, std::string & password, bool & maySave )
{
    return false;
}

void Listener::contextNotify( const char * path, svn_wc_notify_action_t action, svn_node_kind_t kind, const char * mime_type, svn_wc_notify_state_t content_state, svn_wc_notify_state_t prop_state, svn_revnum_t revision )
{
}

#if (SVN_VER_MAJOR >= 1) && (SVN_VER_MINOR >= 2)
void Listener::contextNotify( const svn_wc_notify_t *action )
{
}
#endif

bool Listener::contextCancel( )
{
    return false;
}

bool Listener::contextGetLogMessage( std::string & msg )
{
    return false;
}

svn::ContextListener::SslServerTrustAnswer Listener::contextSslServerTrustPrompt( const SslServerTrustData & data, apr_uint32_t & acceptedFailures )
{
    return DONT_ACCEPT;
}

bool Listener::contextSslClientCertPrompt( std::string & certFile )
{
    return false;
}

bool Listener::contextSslClientCertPwPrompt( std::string & password, const std::string & realm, bool & maySave )
{
    return false;
}
