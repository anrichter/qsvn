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
#include "statustext.h"
#include "svnclient.h"

//SvnCpp
#include "svncpp/client.hpp"
#include "svncpp/status.hpp"

//Qt
#include <QtCore>


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
    svnContext = new svn::Context();
    svnClient = new svn::Client( svnContext );
    listener = new Listener();

    //todo: svnContext->setListener( listener );
}

SvnClient::~SvnClient()
{
    delete listener;
    delete svnClient;
    delete svnContext;
}

svn::StatusEntries SvnClient::status( QString &directory )
{
    QDir dir( directory );
    return svnClient->status( dir.canonicalPath().toLocal8Bit(), false, true, false, false);
}

bool SvnClient::update( QStringList &updateList )
{
    for ( int i = 0; i < updateList.count(); i++ )
    {
        svn::Path svnPath( updateList.at( i ).toLocal8Bit() );
        try
        {
            svnClient->update( svnPath, svn::Revision::HEAD, false );
        }
        catch ( svn::ClientException e )
        {
            StatusText::Exemplar()->outputMessage( QString::fromLocal8Bit( e.message() ) );
            return false;
        }
        StatusText::Exemplar()->outputMessage( "update " + updateList.at( i ) );
    }
    return true;
}

bool SvnClient::checkout( const QString &url, const QString &path )
{
    if ( url.isEmpty() || path.isEmpty() )
        return FALSE;

    try
    {
        svn::Path _path( path.toLocal8Bit() );
        svnClient->checkout( url.toLocal8Bit(), _path, svn::Revision::HEAD, true );
    }
    catch ( svn::ClientException e )
    {
        StatusText::Exemplar()->outputMessage( QString::fromLocal8Bit( e.message() ) );
        return false;
    }
}
