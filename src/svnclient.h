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

#ifndef SVNCLIENT_H
#define SVNCLIENT_H

//QSvn
#include "listener.h"

//SvnCpp
#include "svncpp/client.hpp"
#include "svncpp/context.hpp"
#include "svncpp/status.hpp"

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
    static SvnClient* instance();

    /**
     * Return StatusEntries for a directory
     * @param directory
     * @return
     */
    svn::StatusEntries status( QString &directory );

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
protected:
    SvnClient();
    ~SvnClient();

private:
    static SvnClient* m_instance;

    svn::Context *svnContext;
    svn::Client *svnClient;
    Listener *listener;
};

#endif
