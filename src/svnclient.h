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

#ifndef SVNCLIENT_H
#define SVNCLIENT_H

//Qt
#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>

//Qt classes
class QProcess;


/**
This Class handles interaction with subversion client
 
@author Andreas Richter
*/

class SvnClient : public QObject
{
    Q_OBJECT
public:
    static SvnClient* Exemplar();

    QStringList getProcessStdoutList();
    QStringList getProcessStderrList();

    bool isWorkingCopy( const QString &path );
    QString getMessageString();
    
    //svn calls
    bool add( const QString &path, const QString &filename, bool withOutput = true );
    bool info( const QString &path, bool withOutput = true );
    bool status( const QString &path, bool withOutput = true );
    bool update( const QString &path, bool withOutput = true );
    bool diff( const QString &path, const QString &filename, bool withOutput = true );
    bool checkout( const QString &path, const QString &url, bool withOutput = true );

public slots:
    void readStdoutSlot(); //!< read out the Stdout written from running process
    void readStderrSlot(); //!< read out the Stderr written form running process

protected:
    SvnClient();
    ~SvnClient();

private:
    static SvnClient* _exemplar;

    QProcess *process;
    
    //Process outputs
    QStringList processStdoutList;
    QStringList processStderrList;
    QString messageString; //!< contains some messages for output
    bool immediateOutput; //!< if it is true the svn output arw written immediately to StatusText

    void prepareNewProcess( const QString &workingDirectory = "" ); //!< initialies all for a new process
    bool startProcess( const QString &startErrorMessage );
    bool startAndWaitProcess( const QString &startErrorMessage );
};

#endif
