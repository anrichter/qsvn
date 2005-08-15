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
#include "svnwrapper.h"
#include "config.h"
#include "statustext.h"

//Qt
#include <QDir>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <QUrl>


//make SvnWrapper as a singleton
SvnWrapper* SvnWrapper::_exemplar = 0;

SvnWrapper* SvnWrapper::Exemplar()
{
    if ( _exemplar == 0 )
    {
        _exemplar = new SvnWrapper;
    }
    return _exemplar;
}

//SvnWrapper implementation
SvnWrapper::SvnWrapper()
{
    process = new QProcess( this );
    connect( process, SIGNAL( readyReadStandardOutput() ), this, SLOT( readStdoutSlot() ) );
    connect( process, SIGNAL( readyReadStandardError() ), this, SLOT( readStderrSlot() ) );
    immediateOutput = true;
}

SvnWrapper::~SvnWrapper()
{
    delete process;
    process = 0;
}

void SvnWrapper::prepareNewProcess( const QString &workingDirectory )
{
    processStdoutList.clear();
    processStderrList.clear();
    svnArgumentList.clear();
    messageString = "";
    if ( !workingDirectory.isEmpty() )
        process->setWorkingDirectory( workingDirectory );
}

bool SvnWrapper::startProcess( const QString &startErrorMessage )
{
    if ( immediateOutput )
    {
        //output command line
        QString commandLine = "\n" + Config::Exemplar()->getSvnExecutable() + " ";
        QStringList::Iterator it = svnArgumentList.begin();
        while( it != svnArgumentList.end() )
        {
            commandLine += *it + " ";
            ++it;
        }
        StatusText::Exemplar()->outputMessage( commandLine );
    }
    //start process
    QString svnExecute = Config::Exemplar()->getSvnExecutable();
    process->start( svnExecute, svnArgumentList );
    if ( !process->waitForStarted() )
    {
        messageString = startErrorMessage;
        return FALSE;
    }
    return TRUE;
}

bool SvnWrapper::startAndWaitProcess( const QString &startErrorMessage )
{
    if ( startProcess( startErrorMessage ) )
    {
        if ( !process->waitForFinished() )
            return FALSE;
        return ( process->exitCode() == 0 );
    }
    else
    {
        return FALSE;
    }
}

void SvnWrapper::readStdoutSlot()
{
    QString string = process->readAllStandardOutput();
    if ( !string.isEmpty() )
    {
        processStdoutList.append( string );
        if ( immediateOutput )
            StatusText::Exemplar()->outputMessage( string );
    }
}

void SvnWrapper::readStderrSlot()
{
    QString string = process->readAllStandardError();
    if ( !string.isEmpty() )
    {
        processStderrList.append( string );
        if ( immediateOutput )
            StatusText::Exemplar()->outputMessage( string );
    }
}

QStringList SvnWrapper::getProcessStdoutList()
{
    return processStdoutList;
}

QStringList SvnWrapper::getProcessStderrList()
{
    return processStderrList;
}

QString SvnWrapper::getMessageString()
{
    return messageString;
}

void SvnWrapper::filesToList( int svnCommandType, QStringList *list, const QString &path, const QString pathPrefix )
{
    if ( !list->isEmpty() && !path.isNull() && isWorkingCopy( path ) )
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

            for ( QStringList::Iterator it = statusList.begin(); it != statusList.end(); ++it )
            {
                _lineString = *it;
                _fileName = _lineString.right( _lineString.length() - 6 );
                _fileName = _fileName.simplified(); //convert into simple whitespace seaparatet string

                if ( _lineString.at( 0 ) != '?' )
                {
                    _fileName = _fileName.section( ' ', 3, 3 ); //get FileName
                }

                if ( ( _fileName != "." ) && ( _fileName != ".." ) )
                {
                    if ( ( svnCommandType ==  Commit ) || ( svnCommandType == Revert ) )
                    {
                        if ( ( _lineString.at( 0 ) == 'M' ) ||
                                ( _lineString.at( 0 ) == 'A' ) ||
                                ( _lineString.at( 0 ) == 'D' ) )
                        {
                            list->append( pathPrefix + _fileName );
                        }
                    }
                    else
                        if ( svnCommandType == Remove )
                        {
                            if ( _lineString.at( 0 ) == ' ' )
                            {
                                list->append( pathPrefix + _fileName );
                            }
                        }
                        else
                            if ( svnCommandType == Add )
                            {
                                if ( _lineString.at( 0 ) == '?' )
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

bool SvnWrapper::isWorkingCopy( const QString &path )
{
    QDir dir( path + QDir::separator() + ".svn" );
    if ( dir.exists() )
        return TRUE;
    else
        return FALSE;
}

//svn calls
bool SvnWrapper::doSvnCommand( int svnCommandType, const QString &path, const QStringList *filenameList, QString &commitMessage, bool withOutput )
{
    if ( path.isNull() )
        return FALSE;

    QStringList my_list( *filenameList );
    QStringList::Iterator it;
    bool _return = FALSE;

    immediateOutput = withOutput;

    prepareNewProcess( path );
    switch ( svnCommandType )
    {
    case Add:
        svnArgumentList << "add";
        svnArgumentList << "-N";
        for ( it = my_list.begin(); it != my_list.end(); ++it )
        {
            svnArgumentList << *it;
        }
        _return = startAndWaitProcess( tr( "cannot start svn add" ) );
        break;
    case Commit:
        svnArgumentList << "commit";
        svnArgumentList << "-m";
        svnArgumentList << commitMessage;
        for ( it = my_list.begin(); it != my_list.end(); ++it )
        {
            svnArgumentList << *it;
        }
        _return = startAndWaitProcess( tr( "cannot start svn commit" ) );
        break;
    case Info:
        svnArgumentList << "info";
        svnArgumentList << path;
        _return = startAndWaitProcess( tr( "cannot start svn info - is your svn executable installed and configured in settings?" ) );
        break;
    case Remove:
        svnArgumentList << "remove";
        for ( it = my_list.begin(); it != my_list.end(); ++it )
        {
            svnArgumentList << *it;
        }
        _return = startAndWaitProcess( tr( "cannot start svn remove" ) );
        break;
    case Revert:
        svnArgumentList << "revert";
        for ( it = my_list.begin(); it != my_list.end(); ++it )
        {
            svnArgumentList << *it;
        }
        _return = startAndWaitProcess( tr( "cannot start svn revert" ) );
        break;
    case Status:
        svnArgumentList << "status";
        svnArgumentList << "-vN";
        svnArgumentList << path;
        _return = startAndWaitProcess( tr( "cannot start svn status -v" ));
        break;
    case Update:
        svnArgumentList << "update";
        for ( it = my_list.begin(); it != my_list.end(); ++it )
        {
            svnArgumentList << *it;
        }
        _return = startAndWaitProcess( tr( "cannot start svn update" ) );
        break;
    }
    return _return;
}

bool SvnWrapper::doSvnCommand( int svnCommandType, const QString &path, const QStringList *filenameList, bool withOutput )
{
    QString _commitMessage = "";
    return doSvnCommand( svnCommandType, path, filenameList, _commitMessage, withOutput );
}

bool SvnWrapper::doSvnCommand( int svnCommandType, const QString &path, bool withOutput )
{
    QString _commitMessage = "";
    QStringList *_filenameList = new QStringList;
    return doSvnCommand( svnCommandType, path, _filenameList, _commitMessage, withOutput );
}

bool SvnWrapper::add
    ( const QString &path, const QString &filename, bool withOutput )
{
    if ( !path.isNull() && !filename.isNull() )
    {
        immediateOutput = withOutput;

        prepareNewProcess( path );
        svnArgumentList << "add";
        svnArgumentList << "-N";
        svnArgumentList << filename;

        return startAndWaitProcess( "cannot start svn add" );
    }
    else
        return FALSE;
}

bool SvnWrapper::diff( const QString &path, const QString &filename, bool withOutput )
{
    if ( !path.isNull() && !filename.isNull() )
    {
        immediateOutput = withOutput;

        prepareNewProcess( path );
        svnArgumentList << QString( ".svn/text-base/%1.svn-base" ).arg( filename );
        svnArgumentList << filename;

        process->start( Config::Exemplar()->getDiffViewer(), svnArgumentList );
        if ( !process->waitForStarted() )
        {
            return FALSE;
        }
        return TRUE;
    }
    else
        return FALSE;
}

bool SvnWrapper::diff( const QString &path, const QStringList *filenameList, bool withOutput )
{
    if ( !path.isNull() && !filenameList->isEmpty() && ( filenameList->count() > 0 ) )
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

bool SvnWrapper::diff( const QString &fullFileName, bool withOutput )
{
    if ( !fullFileName.isNull() )
    {
        QFileInfo fileInfo = QFileInfo( fullFileName );
        diff( fileInfo.absolutePath(), fileInfo.fileName(), withOutput );
        return TRUE;
    }
    else
        return FALSE;

}

bool SvnWrapper::checkout( const QString &path, const QString &url, bool withOutput )
{
    if ( !path.isNull() && !url.isNull() )
    {
        immediateOutput = withOutput;

        prepareNewProcess( path );
        svnArgumentList << "checkout";
        svnArgumentList << url;

        return startAndWaitProcess( "cannot start svn checkout" );
    }
    else
        return FALSE;
}
