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
#include "config.h"
#include "listener.h"
#include "statustext.h"
#include "svnclient.h"

//SvnCpp
#include "svnqt/client.hpp"
#include "svnqt/revision.hpp"
#include "svnqt/status.hpp"
#include "svnqt/targets.hpp"

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
    svnClient = svn::Client::getobject( svnContext, 0 );
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
        StatusText::instance()->outputMessage( e.msg() );
        return svn::StatusEntries();
    }
    return svn::StatusEntries();
}

svn::Status SvnClient::singleStatus( QString &path )
{
    try
    {
        return svnClient->singleStatus( path, false, svn::Revision::HEAD );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return svn::Status();
    }
}

bool SvnClient::update( QStringList &updateList )
{
    if ( updateList.isEmpty() )
        return true;

    try
    {
        StatusText::instance()->outputMessage( "" );
        svn::Targets targets( updateList );
        svnClient->update( targets, svn::Revision::HEAD, true, false );
        completedMessage( QString( updateList.at( 0 ) ) );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
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
        completedMessage( path );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
}

bool SvnClient::add
    ( const QStringList &addList )
{
    if ( addList.isEmpty() )
        return true;

    try
    {
        QString file;
        foreach( file, addList )
        {
            svnClient->add
            ( file, false );
        }
        completedMessage( file );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
}

bool SvnClient::revert( const QStringList &revertList )
{
    if ( revertList.isEmpty() )
        return true;

    try
    {
        svn::Targets targets( revertList );
        svnClient->revert( targets, false );
        completedMessage( QString( revertList.at( 0 ) ) );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
}

bool SvnClient::revert( const QString fileName )
{
    QStringList fileList;
    fileList << fileName;
    return revert( fileList );
}

bool SvnClient::commit( const QStringList &commitList, const QString &logMessage )
{
    if ( commitList.isEmpty() )
        return true;

    try
    {
        svn::Targets targets( commitList );
        svnClient->commit( targets, logMessage, false );
        completedMessage( QString( commitList.at( 0 ) ) );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
}

bool SvnClient::remove
    ( const QStringList &removeList )
{
    if ( removeList.isEmpty() )
        return true;

    try
    {
        svn::Targets targets( removeList );
        svnClient->remove
        ( targets, false );
        completedMessage( QString( removeList.at( 0 ) ) );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
}

bool SvnClient::diff( const QString &file )
{
    if ( Config::instance()->getDiffViewer().isEmpty() )
    {
        //diff output to StatusText
        try
        {
            QString delta = svnClient->diff( svn::Path( QDir::tempPath() + QDir::separator() + "qsvn" ), svn::Path( file ), svn::Revision::BASE, svn::Revision::WORKING, true, false, false );
            StatusText::instance()->outputMessage( delta );
        }
        catch ( svn::ClientException e )
        {
            StatusText::instance()->outputMessage( e.msg() );
            return false;
        }
    }
    else
    {
        QFileInfo fileInfo;
        QString baseFile, workFile;

        fileInfo = QFileInfo( file );
        baseFile = QDir::convertSeparators( fileInfo.absolutePath() ) + QDir::separator();
        baseFile = baseFile + QString( QDir::convertSeparators( ".svn/text-base/%1.svn-base" ) ).arg( fileInfo.fileName() );
        workFile = QDir::convertSeparators( fileInfo.absoluteFilePath() );

        QProcess::startDetached( Config::instance()->getDiffViewer(), QStringList() << baseFile << workFile );
    }
    return true;
}

bool SvnClient::diff( const QStringList &fileList )
{
    bool result = true;
    QString file;
    foreach( file, fileList )
    result = result && diff( file );

    return result;
}

const svn::LogEntries* SvnClient::log( const QString &path )
{
    try
    {
        return svnClient->log( path, svn::Revision::HEAD, svn::Revision( ( long ) 0 ), true, false, 0 );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
}

void SvnClient::completedMessage( const QString &path )
{
    QString _path = path;
    svn::Status status = singleStatus( _path );
    if ( status.isVersioned() )
        StatusText::instance()->outputMessage( QString( tr( "Completed at Revision %1\n" ) ).arg( status.entry().revision() ) );
    else
        StatusText::instance()->outputMessage( tr( "Completed\n" ) );
}

void SvnClient::setCancel( )
{
    listener->setCancel( true );
}
