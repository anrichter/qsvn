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
 ***************************************************************************/


//QSvn
#include "listener.h"
#include "statustext.h"
#include "svnclient.h"

//SvnCpp
#include "svncpp/client.hpp"
#include "svncpp/status.hpp"
#include "svncpp/targets.hpp"

//Qt
#include <QtCore>


//make SvnClient a singleton
SvnClient* SvnClient::m_instance = 0;

SvnClient* SvnClient::instance()
{
    if ( m_instance == 0 )
    {
        m_instance = new SvnClient;
    }
    return m_instance;
}

//SvnClient implementation
SvnClient::SvnClient()
{
    svnContext = new svn::Context();
    svnClient = new svn::Client( svnContext );
    listener = new Listener();

    svnContext->setListener( listener );
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
    try
    {
        return svnClient->status( dir.canonicalPath(), false, true, false, false);
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( QString::fromLocal8Bit( e.message() ) );
        return svn::StatusEntries();
    }
    return svn::StatusEntries();
}

bool SvnClient::update( QStringList &updateList )
{
    try
    {
        StatusText::instance()->outputMessage( "" );
        svn::Targets targets( updateList );
        svnClient->update( targets, svn::Revision::HEAD, true, false );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( QString::fromLocal8Bit( e.message() ) );
        return false;
    }
    return true;
}

bool SvnClient::checkout( const QString &url, const QString &path )
{
    if ( url.isEmpty() || path.isEmpty() )
        return false;

    try
    {
        svnClient->checkout( url, path, svn::Revision::HEAD, true );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( QString::fromLocal8Bit( e.message() ) );
        return false;
    }
    return true;
}

bool SvnClient::add( const QStringList &addList )
{
    QString file;
    foreach( file, addList )
    {
        try
        {
            svnClient->add( file, false );
        }
        catch ( svn::ClientException e )
        {
            StatusText::instance()->outputMessage( QString::fromLocal8Bit( e.message() ) );
            return false;
        }
    }
    return true;
}

bool SvnClient::revert( const QStringList &revertList )
{
    try
    {
        svn::Targets targets( revertList );
        svnClient->revert( targets, false );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( QString::fromLocal8Bit( e.message() ) );
        return false;
    }
    return true;
}

bool SvnClient::commit( const QStringList &commitList, const QString &logMessage )
{
    try
    {
        svn::Targets targets( commitList );
        svnClient->commit( targets, logMessage, false );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( QString::fromLocal8Bit( e.message() ) );
        return false;
    }
    return true;
}

bool SvnClient::remove( const QStringList &removeList )
{
    try
    {
        svn::Targets targets( removeList );
        svnClient->remove( targets, false );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( QString::fromLocal8Bit( e.message() ) );
        return false;
    }
    return true;
}
