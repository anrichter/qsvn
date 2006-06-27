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

#ifndef SVNCLIENT_H
#define SVNCLIENT_H

//QSvn
#include "listener.h"

//SvnCpp
#include "svnqt/client.hpp"
#include "svnqt/context.hpp"
#include "svnqt/status.hpp"

//Qt
#include <QtCore>


/**
This Class handles subversion client calls

@author Andreas Richter
*/

class SvnClient : public QObject
{
    Q_OBJECT
public:
    enum SvnAction { SvnNone   = 0x00,
                     SvnAdd    = 0x01,
                     SvnCommit = 0x02,
                     SvnDelete = 0x04,
                     SvnRevert = 0x05 };

    static SvnClient* instance();

    /**
     * Return StatusEntries for a directory
     * @param directory
     * @return
     */
    svn::StatusEntries status( QString &directory );
    svn::Status singleStatus( QString &path );

    /**
     * Update entries from the updateList.
     * @param updateList
     * @return
     */
    bool update( QStringList &updateList );

    /**
     * Checkout a working copy
     * @param url The complete URL from subversion repository
     * @param path The where the working copy will be checked out.
     * @return True if checkout properly otherwise False
     */
    bool checkout( const QString &url, const QString &path );
    bool svnexport( const QString &url, const QString &path, const svn::Revision &revision, const bool verbose = true );

    bool add( const QStringList &addList );
    bool revert( const QStringList &revertList, const bool verbose = true );
    bool revert( const QString fileName, const bool verbose = true );
    bool commit( const QStringList &commitList, const QString &logMessage );
    bool remove( const QStringList &removeList );
    bool diff( const QString &file, const svn::Revision &revisionFrom, const svn::Revision &revisionTo );
    bool diff( const QString &file );
    bool diff( const QStringList &fileList );

    const svn::LogEntries* log( const QString &path, 
                                const svn::Revision &revisionStart, const svn::Revision &revisionEnd, 
                                bool discoverChangedPaths, bool strictNodeHistory,
                                int limit );
    bool cleanup( const QString &path );
    bool resolved( const QString &path );

public slots:
    void setCancel( );

protected:
    SvnClient();
    ~SvnClient();

private:
    static SvnClient* m_instance;

    svn::Context *svnContext;
    svn::Client *svnClient;
    Listener *listener;

    void completedMessage( const QString &path );
};

#endif
