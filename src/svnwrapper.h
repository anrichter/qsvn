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

#ifndef SVNWRAPPER_H
#define SVNWRAPPER_H

//Qt
#include <QObject>
#include <QStringList>

//Qt classes
class QProcess;


/**
This Class handles interaction with subversion client
 
@author Andreas Richter
*/

class SvnWrapper : public QObject
{
    Q_OBJECT
public:
    enum SvnCommandType { Add    = 0x01,
                          Commit = 0x02,
                          Info   = 0x03,
                          Remove = 0x04,
                          Revert = 0x05,
                          Status = 0x06,
                          Update = 0x07 };

    static SvnWrapper* Exemplar();

    QStringList getProcessStdoutList();
    QStringList getProcessStderrList();

    bool isWorkingCopy( const QString &path );
    QString getMessageString();
    void filesToList( int svnCommandType, QStringList *list, const QString &path, const QString pathPrefix = "" );

    bool doSvnCommand( int svnCommandType, const QString &path, const QStringList *filenameList, QString &commitMessage, bool withOutput = true );
    bool doSvnCommand( int svnCommandType, const QString &path, const QStringList *filenameList, bool withOutput = true );
    bool doSvnCommand( int svnCommandType, const QString &path, bool withOutput = true );

    //svn calls
    bool add
        ( const QString &path, const QString &filename, bool withOutput = true );
    bool diff( const QString &path, const QString &filename, bool withOutput = true );
    bool diff( const QString &path, const QStringList *filenameList, bool withOutput = true );
    bool diff( const QString &fullFileName, bool withOutput = true );
    bool checkout( const QString &path, const QString &url, bool withOutput = true );

public slots:
    void readStdoutSlot(); //!< read out the Stdout written from running process
    void readStderrSlot(); //!< read out the Stderr written form running process

protected:
    SvnWrapper();
    ~SvnWrapper();

private:
    static SvnWrapper* _exemplar;

    QProcess *process;

    //Process outputs
    QStringList processStdoutList;
    QStringList processStderrList;
    QStringList svnArgumentList;
    QString messageString; //!< contains some messages for output
    bool immediateOutput; //!< if it is true the svn output arw written immediately to StatusText

    void prepareNewProcess( const QString &workingDirectory = "" ); //!< initialies all for a new process
    bool startProcess( const QString &startErrorMessage );
    bool startAndWaitProcess( const QString &startErrorMessage );
};

#endif
