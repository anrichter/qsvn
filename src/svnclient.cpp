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
#include "showlog.h"
#include "statustext.h"
#include "svnclient.h"

//SvnCpp
#include "svnqt/client.hpp"
#include "svnqt/revision.hpp"
#include "svnqt/status.hpp"
#include "svnqt/targets.hpp"
#include "svnqt/url.hpp"
#include "svnqt/wc.hpp"

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
    if ( !svn::Wc::checkWc( directory ) )
        return svn::StatusEntries();

    QDir dir( directory );
    listener->setVerbose( false );
    try
    {
        return svnClient->status( dir.canonicalPath(), false, true, false, false);
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return svn::StatusEntries();
    }
}

svn::Status SvnClient::singleStatus( QString &path )
{
    listener->setVerbose( false );
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

    svn::Revisions revisions;

    QString path = updateList.at( 0 );
    svn::Status status = singleStatus( path );
    svn_revnum_t fromRevision = status.entry().cmtRev();

    listener->setVerbose( true );
    try
    {
        StatusText::instance()->outputMessage( "" );
        svn::Targets targets( updateList );
        revisions = svnClient->update( targets, svn::Revision::HEAD, true, false );
        completedMessage( QString( updateList.at( 0 ) ) );
        if ( ( updateList.count() == 1 ) &&                 //only for one Entry
               Config::instance()->value( KEY_SHOWLOGAFTERUPDATE ).toBool() &&    //only if configured
             ( !revisions.isEmpty() )                       //only if update results with a non-empty revisions-list
           )
        {
            ShowLog::doShowLog( 0, updateList.at( 0 ), revisions.at( 0 ).revision(), fromRevision );
        }

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

    listener->setVerbose( true );
    try
    {
        svnClient->checkout( url, path, svn::Revision::HEAD );
        completedMessage( path );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
}

bool SvnClient::svnexport( const QString &url, const QString &path, const svn::Revision &revision, const bool verbose )
{
    if ( url.isEmpty() || path.isEmpty() )
        return false;

    listener->setVerbose( verbose );
    try
    {
        svnClient->doExport( url, path, revision );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    listener->setVerbose( true );
    return true;
}

bool SvnClient::add
    ( const QStringList &addList )
{
    if ( addList.isEmpty() )
        return true;

    listener->setVerbose( true );
    try
    {
        QString file;
        foreach( file, addList )
        {
            svnClient->add( file, false );
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

bool SvnClient::revert( const QStringList &revertList, const bool verbose )
{
    if ( revertList.isEmpty() )
        return true;

    listener->setVerbose( verbose );
    try
    {
        svn::Targets targets( revertList );
        svnClient->revert( targets, false );
        if ( verbose )
            completedMessage( QString( revertList.at( 0 ) ) );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
}

bool SvnClient::revert( const QString fileName, const bool verbose )
{
    QStringList fileList;
    fileList << fileName;
    return revert( fileList, verbose );
}

bool SvnClient::commit( const QStringList &commitList, const QString &logMessage )
{
    if ( commitList.isEmpty() )
        return true;

    listener->setVerbose( true );
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

    listener->setVerbose( true );
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

bool SvnClient::diff( const QString &file, const svn::Revision &revisionFrom, const svn::Revision &revisionTo )
{
    if ( Config::instance()->value( KEY_DIFFVIEWER ).isNull() )
    {
        //diff output to StatusText
        listener->setVerbose( true );
        try
        {
            QString delta = svnClient->diff( Config::instance()->tempDir(),
                                             svn::Path( file ),
                                             revisionFrom, revisionTo,
                                             true, false, false, true );
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
        QFileInfo fileInfo( file );
        QString fileFrom, fileTo, dir, filename, ext;
        svn::Path filePath( file );

        filePath.split( dir, filename, ext );

        //fileFrom
        switch ( svn::Revision( revisionFrom ).kind() )
        {
            case svn_opt_revision_base:
                fileFrom = QDir::convertSeparators( fileInfo.absolutePath() ) + QDir::separator();
                fileFrom = fileFrom + QString( QDir::convertSeparators( ".svn/text-base/%1.svn-base" ) ).arg( fileInfo.fileName() );
                break;
            case svn_opt_revision_working:
                fileFrom = QDir::convertSeparators( fileInfo.absoluteFilePath() );
                break;
            default:
                if ( svn::Url::isValid( file ) )
                {
                    fileFrom = Config::instance()->tempDir() + filename + QString( ".%1.%2 " )
                            .arg( int ( revisionFrom.revnum() ) )
                            .arg( ext );
                    if ( !svnexport( file, fileFrom, revisionFrom, false ) )
                        return false;
                } else
                    fileFrom = "";
                break;
        }

        //fileTo
        switch( svn::Revision( revisionTo ).kind() )
        {
            case svn_opt_revision_base:
                fileTo = QDir::convertSeparators( fileInfo.absolutePath() ) + QDir::separator();
                fileTo = fileTo + QString( QDir::convertSeparators( ".svn/text-base/%1.svn-base" ) ).arg( fileInfo.fileName() );
                break;
            case svn_opt_revision_working:
                fileTo = QDir::convertSeparators( fileInfo.absoluteFilePath() );
                break;
            default:
                if ( svn::Url::isValid( file ) )
                {
                    fileTo = Config::instance()->tempDir() + filename + QString( ".%1.%2 " )
                            .arg( int ( revisionTo.revnum() ) )
                            .arg( ext );
                    if ( !svnexport( file, fileTo, revisionTo, false ) )
                        return false;
                } else
                    fileTo = "";
                break;
        }

        QProcess::startDetached( Config::instance()->value( KEY_DIFFVIEWER ).toString(), QStringList() << fileFrom << fileTo );
    }
    return true;
}

bool SvnClient::diff( const QString &file )
{
    return diff( file, svn::Revision::BASE, svn::Revision::WORKING );
}

bool SvnClient::diff( const QStringList &fileList )
{
    bool result = true;
    QString file;
    foreach( file, fileList )
        result = result && diff( file, svn::Revision::BASE, svn::Revision::WORKING );

    return result;
}

const svn::LogEntries* SvnClient::log( const QString &path, const svn::Revision &revisionStart, const svn::Revision &revisionEnd, int limit )
{
    listener->setVerbose( true );
    try
    {
        return svnClient->log( path, revisionStart, revisionEnd, true, false, limit );
    }
    catch ( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
}

bool SvnClient::cleanup( const QString &path )
{
    listener->setVerbose( true );
    try
    {
        svnClient->cleanup( path );
    }
    catch( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
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

bool SvnClient::resolved( const QString & path )
{
    listener->setVerbose( true );
    try
    {
        svnClient->resolved( path, false );
    }
    catch( svn::ClientException e )
    {
        StatusText::instance()->outputMessage( e.msg() );
        return false;
    }
    return true;
}
