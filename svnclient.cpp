/***************************************************************************
 *   Copyright (C) 2004 by Andreas Richter                                 *
 *   ar@oszine.de                                                          *
 *   http://www.oszine.de                                                  *
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
#include "svnclient.h"

//Qt
#include <qapplication.h>
#include <qprocess.h>

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
    svnCommand = "svn"; //todo: make the svn executable configurable over the config-dialog
    process = new QProcess();
    connect( process, SIGNAL( readyReadStdout() ), this, SLOT( readStdoutSlot() ) );
    connect( process, SIGNAL( readyReadStderr() ), this, SLOT( readStderrSlot() ) );
}

SvnClient::~SvnClient()
{
    delete process;
    process = 0;
}

void SvnClient::prepareNewProcess()
{
    processStdout = "";
    processStderr = "";
    process->clearArguments();
    process->addArgument( svnCommand );
}

bool SvnClient::isWorkingCopy( const QString &path )
{
    prepareNewProcess();
    process->addArgument( "info" );
    process->addArgument( path );
    if ( !process->start() )
    {
        qDebug( "cannot start svn info" );
    }
    while ( process->isRunning() )
    {
        #ifdef Q_WS_X11
        sleep( 1 );
        #endif

        #ifdef Q_WS_WIN
        Sleep( 1 );
        #endif
    }
    return processStderr == "" ;
}

void SvnClient::readStdoutSlot()
{
    QByteArray data = process->readStdout();
    processStdout = QString( data );
}

void SvnClient::readStderrSlot()
{
    QByteArray data = process->readStderr();
    processStderr = QString( data );
}

QString SvnClient::getProcessStdout()
{
    return processStdout;
}

QString SvnClient::getProcessStderr()
{
    return processStderr;
}
