/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2005 Andreas Richter <ar@oszine.de>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
#include "config.h"
#include "statustext.h"
#include "workingcopy.h"

//Qt
#include <qapplication.h>
#include <qprocess.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qurl.h>

//Std
#ifdef Q_WS_X11
#include <unistd.h>
#endif

#ifdef Q_WS_WIN
#include <windows.h>
#endif


//make SvnClient as a singleton
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
    process = new QProcess();
    connect( process, SIGNAL( readyReadStdout() ), this, SLOT( readStdoutSlot() ) );
    connect( process, SIGNAL( readyReadStderr() ), this, SLOT( readStderrSlot() ) );
    immediateOutput = true;
}

SvnClient::~SvnClient()
{
    delete process;
    process = 0;
}

void SvnClient::prepareNewProcess( const QString &workingDirectory )
{
    processStdoutList.clear();
    processStderrList.clear();
    messageString = "";
    process->clearArguments();
    process->addArgument( Config::Exemplar()->getSvnExecutable() );
    if ( workingDirectory != "" )
        process->setWorkingDirectory( QDir( workingDirectory ) );
}

bool SvnClient::startProcess( const QString &startErrorMessage )
{
    if ( immediateOutput )
    {
        //output command line
        QString commandLine = "\n";
        QStringList list = process->arguments();
        QStringList::Iterator it = list.begin();
        while( it != list.end() ) {
            commandLine += *it + " ";
            ++it;
        }
        StatusText::Exemplar()->outputMessage( commandLine );
    }

    //start process
    if ( !process->start() )
    {
        messageString = startErrorMessage;
        return FALSE;
    }
    return TRUE;
}

bool SvnClient::startAndWaitProcess( const QString &startErrorMessage )
{
    if ( startProcess( startErrorMessage ) )
    {
        while ( process->isRunning() )
        {
#ifdef Q_WS_X11
            sleep( 1 );
#endif

#ifdef Q_WS_WIN

            Sleep( 1 );
#endif
            //read out stdout and strerr
            readStdoutSlot();
            readStderrSlot();
        }
        return process->normalExit();
    }
    else
    {
        return FALSE;
    }
}

void SvnClient::readStdoutSlot()
{
    QString string = process->readLineStdout();
    while ( string )
    {
        processStdoutList.append( string );
        if ( immediateOutput )
            StatusText::Exemplar()->outputMessage( string );
        string = process->readLineStdout();
    }
}

void SvnClient::readStderrSlot()
{
    QString string = process->readLineStderr();
    while ( string )
    {
        processStderrList.append( string );
        if ( immediateOutput )
            StatusText::Exemplar()->outputMessage( string );
        string = process->readLineStderr();
    }
}

QStringList SvnClient::getProcessStdoutList()
{
    return processStdoutList;
}

QStringList SvnClient::getProcessStderrList()
{
    return processStderrList;
}

QString SvnClient::getMessageString()
{
    return messageString;
}

void SvnClient::filesToList( int svnCommandType, QStringList *list, const QString &path, const QString pathPrefix )
{
    if ( list && path && isWorkingCopy( path ) )
    {
        if ( svnCommandType == Update )
        {
            list->append( "." );
            return;            
        }
        
        if ( doSvnCommand( Status, path, false ) )
        {
            QStringList statusList( getProcessStdoutList() );
            QString _lineString;
            QString _fileName;
            int i = 0;
            
            for ( QStringList::Iterator it = statusList.begin(); it != statusList.end(); ++it )
            {
                _lineString = *it;
                _fileName = _lineString.right( _lineString.length() - 6 );
                _fileName = _fileName.simplifyWhiteSpace(); //convert into simple whitespace seaparatet string

                if ( _lineString.at( 0 ).latin1() != '?' )
                {
                    _fileName = _fileName.section( ' ', 3, 3 ); //get FileName    
                } 

                if ( ( _fileName != "." ) && ( _fileName != ".." ) )
                {
                    i = int( _lineString.at( 0 ).latin1() );
                    if ( ( svnCommandType ==  Commit ) || ( svnCommandType == Revert ) )
                    {
                        if ( ( i == int( 'M' ) ) || ( i == int( 'A' ) ) || ( i == int( 'D' ) ) )
                        {
                            list->append( pathPrefix + _fileName );
                        }
                    }
                    else 
                    if ( svnCommandType == Remove )
                    {
                        if ( i == int( ' ' ) )
                        {
                            list->append( pathPrefix + _fileName );
                        }
                    }
                    else
                    if ( svnCommandType == Add )
                    {
                        if ( i == int( '?' ) )
                        {
                            list->append( pathPrefix + _fileName );
                        }
                    }
                    
                    //recursive call for subdirectories
                    if ( QDir( path + QDir::separator() + _fileName ).exists() ) // only call when _fileName is a directory 
                    {
                        filesToList( svnCommandType, list, path + QDir::separator() + _fileName, pathPrefix + _fileName + QDir::separator() );
                    }
                }
            }
        }
    }
}
    
bool SvnClient::isWorkingCopy( const QString &path )
{
    QDir dir( path + QDir::separator() + ".svn" );
    if ( dir.exists() )
        return TRUE;
    else
        return FALSE;
}

//svn calls
bool SvnClient::doSvnCommand( int svnCommandType, const QString &path, const QStringList *filenameList, QString &commitMessage, bool withOutput )
{
    if ( !path )
        return FALSE;
        
    QStringList my_list( *filenameList );
    bool _return = FALSE;
    
    immediateOutput = withOutput;

    prepareNewProcess( path );
    switch ( svnCommandType )
    {
    case Add:
        process->addArgument( "add" );
        process->addArgument( "-N" );
        for ( QStringList::Iterator it = my_list.begin(); it != my_list.end(); ++it )
        {
            process->addArgument( *it );
        }
        _return = startAndWaitProcess( tr( "cannot start svn add" ) );
        break;
    case Commit:
        process->addArgument( "commit" );
        process->addArgument( "-m" );
        process->addArgument( commitMessage );
        for ( QStringList::Iterator it = my_list.begin(); it != my_list.end(); ++it )
        {
            process->addArgument( *it );
        }
        _return = startAndWaitProcess( tr( "cannot start svn commit" ) );
        break;
    case Info:
        process->addArgument( "info" );
        process->addArgument( path );
        _return = startAndWaitProcess( tr( "cannot start svn info - is your svn executable installed and configured in settings?" ) );
        break;
    case Remove:
        process->addArgument( "remove" );
        for ( QStringList::Iterator it = my_list.begin(); it != my_list.end(); ++it )
        {
            process->addArgument( *it );
        }
        _return = startAndWaitProcess( tr( "cannot start svn remove" ) );
        break;
    case Revert:
        process->addArgument( "revert" );
        for ( QStringList::Iterator it = my_list.begin(); it != my_list.end(); ++it )
        {
            process->addArgument( *it );
        }
        _return = startAndWaitProcess( tr( "cannot start svn revert" ) );
        break;
    case Status:
        process->addArgument( "status" );
        process->addArgument( "-vN" );
        _return = startAndWaitProcess( tr( "cannot start svn status -v" ));
        break;
    case Update:
        process->addArgument( "update" );
        for ( QStringList::Iterator it = my_list.begin(); it != my_list.end(); ++it )
        {
            process->addArgument( *it );
        }
        _return = startAndWaitProcess( tr( "cannot start svn update" ) );
        break;
    }
    return _return;
}

bool SvnClient::doSvnCommand( int svnCommandType, const QString &path, const QStringList *filenameList, bool withOutput )
{
    QString _commitMessage = "";
    return doSvnCommand( svnCommandType, path, filenameList, _commitMessage, withOutput );
}

bool SvnClient::doSvnCommand( int svnCommandType, const QString &path, bool withOutput )
{
    QString _commitMessage = "";
    QStringList *_filenameList = new QStringList;
    return doSvnCommand( svnCommandType, path, _filenameList, _commitMessage, withOutput );
}

bool SvnClient::add( const QString &path, const QString &filename, bool withOutput )
{
    if ( path && filename )
    {
        immediateOutput = withOutput;

        prepareNewProcess( path );
        process->addArgument( "add" );
        process->addArgument( "-N" );
        process->addArgument( filename );

        return startAndWaitProcess( "cannot start svn add" );
    }
    else
        return FALSE;
}

bool SvnClient::diff( const QString &path, const QString &filename, bool withOutput )
{
    if ( path && filename )
    {
        immediateOutput = withOutput;

        prepareNewProcess( path );
        process->clearArguments();
        process->setWorkingDirectory( path );
        process->addArgument( Config::Exemplar()->getDiffViewer() );
        process->addArgument( QString( ".svn/text-base/%1.svn-base" ).arg( filename ) );
        process->addArgument( filename );

        return startProcess( "cannot start DiffViewer" );
    }
    else
        return FALSE;
}

bool SvnClient::diff( const QString &path, const QStringList *filenameList, bool withOutput )
{
    if ( path && filenameList && ( filenameList->count() > 0 ) )
    {
        bool b = TRUE;
        QStringList my_list( *filenameList );
        for ( QStringList::Iterator it = my_list.begin(); it != my_list.end(); ++it )
        {
            b = b && diff( path, *it, withOutput );
        }
        return b;
    }
    else
        return FALSE;
}

bool SvnClient::diff( const QString &fullFileName, bool withOutput )
{
    if ( fullFileName )
    {
        QUrl url( fullFileName );
        url.dirPath();
        QString path = url.dirPath();
        QString filename = url.fileName();
        
        diff( path, filename, withOutput );
        return TRUE;
    }
    else
        return FALSE;

}

bool SvnClient::checkout( const QString &path, const QString &url, bool withOutput )
{
    if ( path && url )
    {
        immediateOutput = withOutput;

        prepareNewProcess( path );
        process->addArgument( "checkout" );
        process->addArgument( url );

        return startAndWaitProcess( "cannot start svn checkout" );
    }
    else
        return FALSE;
}
