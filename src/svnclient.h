/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2009 Andreas Richter <ar@anrichter.net>                 *
 *                                                                              *
 *   This program is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License Version 2             *
 *   as published by the Free Software Foundation.                              *
 *                                                                              *
 *   This program is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 *   GNU General Public License for more details.                               *
 *                                                                              *
 *   You should have received a copy of the GNU General Public License          *
 *   along with this program; if not, write to the                              *
 *   Free Software Foundation, Inc.,                                            *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                  *
 *                                                                              *
 *******************************************************************************/

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
            SvnNone        = 0x00,
            SvnAdd         = 0x01,
            SvnCommit      = 0x02,
            SvnDelete      = 0x04,
            SvnRevert      = 0x05,
            SvnRename      = 0x06,
            SvnMove        = 0x07,
            SvnCopy        = 0x08,
            SvnMkDir       = 0x09,
            RemoveFromDisk = 0x10
        };

        static SvnClient* instance();

        /**
         * Return StatusEntries for a directory
         * @param directory
         * @return
         */
        svn::StatusEntries status(const QString& path,
                                  svn::Depth depth,
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

        bool svnexport(const QString &url, const QString &path, const svn::Revision &revision, const bool verbose = true);

        bool add(const QString &path, svn::Depth depth);
        bool add(const QStringList &addList, svn::Depth depth);
        bool revert(const QStringList &revertList, const bool verbose = true);
        bool revert(const QString fileName, const bool verbose = true);
        bool commit(const QStringList &commitList, const QString &logMessage);
        bool remove(const QStringList &removeList);
        bool diff(const QString &fileFrom, const QString &fileTo, const svn::Revision &revisionFrom, const svn::Revision &revisionTo);
        bool diffBASEvsWORKING(const QString &file);
        bool diffBASEvsWORKING(const QStringList &fileList);

        bool cleanup(const QString &path);
        bool resolved(const QString &path);
        bool move(const QString &srcPath, const QString &destPath, bool force);
        bool copy(const QString &srcPath, const QString &destPath);
        bool mkdir(const QString &destPath);

        bool merge(const QString &fromUrl, const svn::Revision &fromRevision,
                   const QString &toUrl, const svn::Revision &toRevision,
                   const QString &wcPath, bool force, bool recurse,
                   bool notice_ancestry = false,
                   bool dry_run = false);

        const svn::LogEntriesPtr log(const QString &path,
                                     const svn::Revision &revisionStart,
                                     const svn::Revision &revisionEnd,
                                     const svn::Revision &revisionPeg,
                                     bool discoverChangedPaths,
                                     bool strictNodeHistory,
                                     int limit);

        //Properties
        svn::PathPropertiesMapListPtr propList(const QString &path,
                                               const svn::Revision &revision,
                                               const svn::Revision &peg,
                                               svn::Depth depth = svn::DepthEmpty) ;
        bool propSet(const svn::PropertiesMap propMap,
                     const QString &path,
                     svn::Depth depth,
                     const svn::Revision &revision);
        bool propSet(const QString &propName,
                     const QString &propValue,
                     const svn::Path &path,
                     svn::Depth depth,
                     const svn::Revision &revision,
                     bool skip_checks = false);
        bool propDel(const QString &propName,
                     const svn::Path &path,
                     svn::Depth depth,
                     const svn::Revision &revision);
        bool revPropSet(const QString &propName,
                        const QString &propValue,
                        const svn::Path &path,
                        const svn::Revision &revision,
                        bool force = false);

        bool removeFromDisk(const QStringList &pathList);

        bool isInProgress(); //true, if svn update is running
        QString getLastErrorMessage() const;

        //Helper
        const QString getUUID(const QString &path);
        const QString getSvnActionName(const SvnAction action);

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
        QString getFileRevisionPath(const QString &file, const svn::Revision &revision);
        QString m_lastErrorMessage;
};

#endif
