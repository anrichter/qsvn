/***************************************************************************
 *   This file is part of QSvn Project http://ar.oszine.de/projects/qsvn   *
 *   Copyright (c) 2004-2007 Andreas Richter <ar@oszine.de>                *
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
class Listener;

//SvnCpp
#include "svnqt/client.hpp"

//Qt
#include <QObject>


/**
This Class handles subversion client calls

@author Andreas Richter
*/

class SvnClient : public QObject
{
        Q_OBJECT

    public:
        enum SvnAction
        {
            SvnNone   = 0x00,
            SvnAdd    = 0x01,
            SvnCommit = 0x02,
            SvnDelete = 0x04,
            SvnRevert = 0x05,
            SvnRename = 0x06,
            SvnMove   = 0x07,
            SvnCopy   = 0x08,
            SvnMkDir  = 0x09
        };

        static SvnClient* instance();

        /**
         * Return StatusEntries for a directory
         * @param directory
         * @return
         */
        svn::StatusEntries status(const QString& path,
                                  const bool descend = false,
                                  const bool get_all = true,
                                  const bool update = false,
                                  const bool no_ignore = false,
                                  const svn::Revision revision = svn::Revision::HEAD,
                                  bool detailed_remote = false,
                                  const bool hide_externals = false);

        svn::StatusPtr singleStatus(const QString &path);

        /**
         * Update entries from the updateList.
         * @param updateList
         * @return
         */
        bool update(QStringList updateList, const bool isFileList);

        /**
         * Checkout a working copy
         * @param url The complete URL from subversion repository
         * @param path The where the working copy will be checked out.
         * @return True if checkout properly otherwise False
         */
        bool checkout(const QString &url, const QString &path);
        bool svnexport(const QString &url, const QString &path, const svn::Revision &revision, const bool verbose = true);

        bool add(const QStringList &addList);
        bool revert(const QStringList &revertList, const bool verbose = true);
        bool revert(const QString fileName, const bool verbose = true);
        bool commit(const QStringList &commitList, const QString &logMessage);
        bool remove(const QStringList &removeList);
        bool diff(const QString &file, const svn::Revision &revisionFrom, const svn::Revision &revisionTo);
        bool diff(const QString &file);
        bool diff(const QStringList &fileList);

        const svn::LogEntriesPtr log(const QString &path,
                                   const svn::Revision &revisionStart, const svn::Revision &revisionEnd,
                                   bool discoverChangedPaths, bool strictNodeHistory,
                                   int limit);
        bool cleanup(const QString &path);
        bool resolved(const QString &path);
        bool move(const QString &srcPath, const QString &destPath, bool force);
        bool copy(const QString &srcPath, const QString &destPath);
        bool mkdir(const QString &destPath);

        bool isInProgress(); //true, if svn update is running

    public slots:
        void setCancel();

    protected:
        SvnClient();
        ~SvnClient();

    private:
        static SvnClient* m_instance;

        svn::ContextP svnContext;
        svn::Client *svnClient;
        Listener *listener;
        bool m_InProgress; //true, if svn update is running

        void completedMessage(const QString &path);
};

#endif
