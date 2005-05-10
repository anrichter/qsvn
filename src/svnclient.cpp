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
#include "svnclient.h"

//Qt
#include <qdir.h>
#include <qstring.h>
#include <qurl.h>

//Subversion
#include <svn_client.h>
#include <svn_opt.h>
#include <svn_pools.h>
#include <svn_types.h>


//make SvnClient a singleton
SvnClient* SvnClient::_exemplar = 0;

SvnClient* SvnClient::Exemplar()
{
    if ( _exemplar == 0 )
    {
        _exemplar = new SvnClient;
    }
    return _exemplar;
}

//SvnClient implementation
SvnClient::SvnClient()
{
}

SvnClient::~SvnClient()
{
}

bool SvnClient::checkout( const QString &url, const QString &path )
{
    //check path
    QUrl __url( url );
    QDir __dir( path + QDir::separator() + __url.fileName() );
    if ( !__dir.exists() ) 
    {
        __dir.mkdir( __dir.absPath() );
    }
    
    //svn_revnum_t revnum = -1;
    svn_opt_revision_t revision;
    svn_client_ctx_t *ctx;
    
    apr_pool_initialize ();
    apr_pool_t *pool = svn_pool_create( NULL );
    
    revision.kind = svn_opt_revision_head;
    
    svn_client_create_context( &ctx, pool );
    
    svn_client_checkout( NULL, url.latin1(), __dir.absPath().latin1(), &revision, true, ctx, pool );
    
    svn_pool_destroy( pool );
    
    return TRUE;
}
