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

bool SvnClient::isWorkingCopy( const QString &path )
{
    QDir dir( path + QDir::separator() + ".svn" );
    if ( dir.exists() )
        return TRUE;
    else
        return FALSE;
}


//svn calls
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

bool SvnClient::info( const QString &path, bool withOutput )
{
    if ( path )
    {
        immediateOutput = withOutput;

        prepareNewProcess();
        process->addArgument( "info" );
        process->addArgument( path );
        
        return startAndWaitProcess( "cannot start svn info - is your svn executable installed and configured in settings?" );
    }
    else
    {
        StatusText::Exemplar()->outputMessage( tr( "no path for SvnClient::isWorkingCopy" ) );
        return FALSE;
    }
}

bool SvnClient::status( const QString &path, bool withOutput )
{
    if ( path )
    {
        immediateOutput = withOutput;
        
        prepareNewProcess( path );
        process->addArgument( "status" );
        process->addArgument( "-vN" );
        
        return startAndWaitProcess( "cannot start svn status -v" );
    }
    else
    {
        StatusText::Exemplar()->outputMessage( tr( "no path for SvnClient::getStatus" ) );
        return FALSE;
    }
}

bool SvnClient::update( const QString &path, bool withOutput )
{
    if ( path )
    {
        immediateOutput = withOutput;

        prepareNewProcess();
        process->addArgument( "update" );
        process->addArgument( path );
        
        return startAndWaitProcess( "cannot start svn update" );
    }
    else
    {
        StatusText::Exemplar()->outputMessage( tr( "no path for SvnClient::update" ) );
        return FALSE;
    }
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
