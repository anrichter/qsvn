/***************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2007 Andreas Richter <ar@anrichter.net>                *
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


//QSvn
#include "config.h"
#include "listener.h"
#include "showlog.h"
#include "statustext.h"
#include "svnclient.h"

//SvnCpp
#include "svnqt/client.hpp"
#include "svnqt/revision.hpp"
#include "svnqt/status.hpp"
#include "svnqt/targets.hpp"
#include "svnqt/url.hpp"
#include "svnqt/wc.hpp"

//Qt
#include <QtCore>
#include <QtGui>


//make SvnClient a singleton
SvnClient* SvnClient::m_instance = 0;

SvnClient* SvnClient::instance()
{
    if (m_instance == 0)
        m_instance = new SvnClient;

    return m_instance;
}

//SvnClient implementation
SvnClient::SvnClient()
{
    svnContext = new svn::Context();
    svnClient = svn::Client::getobject(svnContext, 0);
    listener = new Listener(this);

    svnContext->setListener(listener);
    m_InProgress = false;
}

SvnClient::~SvnClient()
{
    delete listener;
    delete svnClient;
    delete svnContext;
}

bool SvnClient::isInProgress()
{
    return m_InProgress;
}

svn::StatusEntries SvnClient::status(const QString& path,
                                     const bool descend,
                                     const bool get_all,
                                     const bool update,
                                     const bool no_ignore,
                                     const svn::Revision revision,
                                     bool detailed_remote,
                                     const bool hide_externals)
{
    if (!svn::Wc::checkWc(path))
        return svn::StatusEntries();

    QDir dir(path);
    listener->setVerbose(false);
    try
    {
        return svnClient->status(dir.canonicalPath(), descend?svn::DepthInfinity:svn::DepthEmpty, get_all, update,
                                 no_ignore, revision, detailed_remote,
                                 hide_externals);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return svn::StatusEntries();
    }
}

svn::StatusPtr SvnClient::singleStatus(const QString &path)
{
    listener->setVerbose(false);
    try
    {
        return svnClient->singleStatus(path, false, svn::Revision::HEAD);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return svn::StatusPtr(new svn::Status());
    }
}

bool SvnClient::update(QStringList updateList, const bool isFileList)
{
    if (updateList.isEmpty())
        return true;

    svn::Revision fromRevision;
    svn::Revisions toRevisions;
    bool result = true;

    foreach (QString path, updateList)
    {
        fromRevision = singleStatus(path)->entry().cmtRev();
        try
        {
            StatusText::out(tr("\nUpdate '%1'").arg(path));
            listener->setVerbose(true);
            m_InProgress = true;
            toRevisions = svnClient->update(path, svn::Revision::HEAD, svn::DepthInfinity, false, false, true);
            m_InProgress = false;
            if (Config::instance()->value(KEY_SHOWLOGAFTERUPDATE).toBool() &&
                !toRevisions.isEmpty() )
            {
                ShowLog::doShowLog(0, path,
                                   toRevisions.at(0).revision(),
                                           fromRevision);
            }
        }
        catch (svn::ClientException e)
        {
            StatusText::out(e.msg());
            result = false;
        }
    }
}

bool SvnClient::checkout(const QString &url, const QString &path)
{
    if (url.isEmpty() || path.isEmpty())
        return false;

    StatusText::out(tr("\nCheckout '%1' into '%2'").arg(url).arg(path));
    listener->setVerbose(true);
    try
    {
        svnClient->checkout(url, path, svn::Revision::HEAD);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

bool SvnClient::svnexport(const QString &url, const QString &path,
                          const svn::Revision &revision, const bool verbose)
{
    if (url.isEmpty() || path.isEmpty())
        return false;

    if (verbose)
        StatusText::out(tr("\nExport '%1' to '%2'").arg(url).arg(path));
    listener->setVerbose(verbose);
    try
    {
        svnClient->doExport(url, path, revision, revision, true);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    listener->setVerbose(true);
    return true;
}

bool SvnClient::add(const QStringList &addList)
{
    if (addList.isEmpty())
        return true;

    listener->setVerbose(true);
    try
    {
        QString file;
        foreach (file, addList)
        {
            svnClient->add(file, svn::DepthInfinity);
        }
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

bool SvnClient::revert(const QStringList &revertList, const bool verbose)
{
    if (revertList.isEmpty())
        return true;

    listener->setVerbose(verbose);
    try
    {
        svn::Targets targets(revertList);
        svnClient->revert(targets, svn::DepthEmpty);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

bool SvnClient::revert(const QString fileName, const bool verbose)
{
    QStringList fileList;
    fileList << fileName;
    return revert(fileList, verbose);
}

bool SvnClient::commit(const QStringList &commitList, const QString &logMessage)
{
    if (commitList.isEmpty())
        return true;

    StatusText::out(tr("\nStart Commit"));
    listener->setVerbose(true);
    try
    {
        svn::Targets targets(commitList);
        svnClient->commit(targets, logMessage, svn::DepthInfinity);
        completedMessage(commitList.at(0));
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

bool SvnClient::remove(const QStringList &removeList)
{
    if (removeList.isEmpty())
        return true;

    listener->setVerbose(true);
    try
    {
        svn::Targets targets(removeList);
        svnClient->remove(targets, false);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

QString SvnClient::getFileRevisionPath(const QString &file, const svn::Revision &revision)
{
    QString _result, _file, _base, _ext, _dir;
    svn::Path _path(file);
    switch (svn::Revision(revision).kind())
    {
        case svn_opt_revision_base:
            _path.split(_dir, _file);
            _result = _dir +
                    QDir::separator() +
                    QString(".svn/text-base/%1.svn-base").arg(_file);
            _result = QDir::toNativeSeparators(_result);
            break;
        case svn_opt_revision_working:
            _result = _path.native();
            break;
        default:
            if (svn::Url::isValid(file))
            {
                _path.split(_dir, _file, _ext);
                _result = Config::instance()->tempDir() +
                        _file + QString(".%1.%2")
                        .arg(int(revision.revnum()))
                        .arg(_ext);
                if (!svnexport(file, _result, revision, false))
                    _result.clear();
            }
            break;
    }
    return _result;
}

bool SvnClient::diff(const QString &fileFrom, const QString &fileTo, const svn::Revision &revisionFrom, const svn::Revision &revisionTo)
{
    if (Config::instance()->value(KEY_DIFFVIEWER).toString().isEmpty())
    {
        //diff output to StatusText
        listener->setVerbose(true);
        try
        {
            QString delta = svnClient->diff(svn::Path(Config::instance()->tempDir()),
                                            svn::Path(fileFrom), svn::Path(fileTo),svn::Path(),
                                            revisionFrom, revisionTo,
                                            svn::DepthInfinity, false, false, true );
            if (delta.isEmpty())
                StatusText::out(tr("There are no differences."));
            else
                StatusText::out(delta);
        }
        catch (svn::ClientException e)
        {
            StatusText::out(e.msg());
            return false;
        }
    }
    else
    {
        QString _fileFrom, _fileTo;
        _fileFrom = getFileRevisionPath(fileFrom, revisionFrom);
        _fileTo = getFileRevisionPath(fileTo, revisionTo);
        if (!QProcess::startDetached(Config::instance()->value(KEY_DIFFVIEWER).toString(),
             QStringList() << _fileFrom << _fileTo))
        {
            QMessageBox::critical(0, tr("Error"),
                                  QString(tr("Can't start %1"))
                                          .arg(Config::instance()->value(KEY_DIFFVIEWER).toString()));
        }
    }
    return true;
}

bool SvnClient::diffBASEvsWORKING(const QString &file)
{
    return diff(file, file, svn::Revision::BASE, svn::Revision::WORKING);
}

bool SvnClient::diffBASEvsWORKING(const QStringList &fileList)
{
    bool result = true;
    QString file;
    foreach (file, fileList)
    result = result && diff(file, file, svn::Revision::BASE, svn::Revision::WORKING);

    return result;
}

const svn::LogEntriesPtr SvnClient::log(const QString &path,
                                        const svn::Revision &revisionStart,
                                        const svn::Revision &revisionEnd,
                                        const svn::Revision &revisionPeg,
                                        bool discoverChangedPaths,
                                        bool strictNodeHistory,
                                        int limit )
{
    listener->setVerbose(true);
    try
    {
        return svnClient->log(path,
                              revisionStart,
                              revisionEnd,
                              revisionPeg,
                              discoverChangedPaths,
                              strictNodeHistory,
                              limit);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return new svn::LogEntries();
    }
}

bool SvnClient::cleanup(const QString &path)
{
    listener->setVerbose(true);
    try
    {
        StatusText::out(tr("\nCleanup '%1'").arg(path));
        svnClient->cleanup(path);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    completedMessage("");
    return true;
}

void SvnClient::completedMessage(const QString &path)
{
    if (!path.isEmpty())
    {
        QString _path = path;
        svn::StatusPtr status = singleStatus(_path);
        if (status->isVersioned())
            StatusText::out(QString(tr("Completed at Revision %1\n"))
                    .arg(status->entry().revision()));
        else
            StatusText::out(tr("Completed\n"));
    }
    else
        StatusText::out(tr("Completed\n"));
}

void SvnClient::setCancel()
{
    listener->setCancel(true);
}

bool SvnClient::resolved(const QString &path)
{
    listener->setVerbose(true);
    try
    {
        svnClient->resolved(path, false);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

bool SvnClient::move(const QString &srcPath, const QString &destPath,
                     bool force)
{
    listener->setVerbose(true);
    try
    {
        svnClient->move(srcPath, destPath, force);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

bool SvnClient::copy(const QString &srcPath, const QString &destPath)
{
    listener->setVerbose(true);
    try
    {
        svnClient->copy(srcPath, svn::Revision::WORKING, destPath);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

bool SvnClient::mkdir(const QString & destPath)
{
    const QString message = "";
    listener->setVerbose(true);
    try
    {
        svnClient->mkdir(svn::Path(destPath), message);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
    return true;
}

svn::PathPropertiesMapListPtr SvnClient::propList(const QString &path,
        const svn::Revision &revision, const svn::Revision &peg, svn::Depth depth)
{
    listener->setVerbose(true);
    try
    {
        return svnClient->proplist(path, revision, peg, depth);
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return svn::PathPropertiesMapListPtr();
    }
}

bool SvnClient::propSet(const svn::PropertiesMap propMap,
                        const QString &path,
                        svn::Depth depth,
                        const svn::Revision &revision)
{
    bool result = true;
    //remove all exist properties
    svn::PathPropertiesMapListPtr _propList;
    _propList = propList(path, revision, revision);
    if(!_propList->isEmpty())
    {
        svn::PathPropertiesMapEntry entry = _propList->at(0);
        svn::PropertiesMap _oldPropMap = entry.second;
        QMapIterator<QString, QString> _oldIter(_oldPropMap);
        while (_oldIter.hasNext())
        {
            _oldIter.next();
            result = result && propDel(_oldIter.key(), path, svn::DepthEmpty, revision);
        }
    }

    //write new properties
    QMapIterator<QString, QString> _iter(propMap);
    while (_iter.hasNext())
    {
        _iter.next();
        result = result && propSet(_iter.key(), _iter.value(), path, depth, revision);
    }
    return result;
}

bool SvnClient::propSet(const QString &propName,
                        const QString &propValue,
                        const svn::Path &path,
                        svn::Depth depth,
                        const svn::Revision &revision,
                        bool skip_checks)
{
    listener->setVerbose(true);
    try
    {
        svnClient->propset(propName, propValue, path, depth, skip_checks, revision);
        return true;
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
}

bool SvnClient::propDel(const QString &propName,
                        const svn::Path &path,
                        svn::Depth depth,
                        const svn::Revision &revision)
{
    listener->setVerbose(true);
    try
    {
        //todo: adjust function-header to propset-header
        svnClient->propdel(propName, path, depth, revision);
        return true;
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
}

bool SvnClient::removeFromDisk(const QStringList &pathList)
{
    QFileInfo fileInfo;
    QDir dir;
    bool success = false, result = true;
    foreach(QString path, pathList)
    {
        fileInfo = QFileInfo(path);
        if (fileInfo.isDir())
        {
            success = dir.rmdir(fileInfo.absolutePath());
        } else if (fileInfo.isFile()) {
            success = QFile::remove(fileInfo.absoluteFilePath());
        }
        if (success)
            StatusText::out(QString(tr("removed from disk: %1")).arg(fileInfo.absoluteFilePath()));
        else
            StatusText::out(QString(tr("Error while remove from disk: %1")).arg(fileInfo.absoluteFilePath()));
        result = result && success;
    }
    return result;
}

bool SvnClient::merge(const QString &fromUrl, const svn::Revision &fromRevision,
                      const QString &toUrl, const svn::Revision &toRevision,
                      const QString & wcPath, bool force, bool recurse,
                      bool notice_ancestry, bool dry_run)
{
    listener->setVerbose(true);
    try
    {
        svnClient->merge(fromUrl, fromRevision,
                         toUrl, toRevision,
                         wcPath, force, recurse?svn::DepthInfinity:svn::DepthFiles,
                         notice_ancestry, dry_run);
        return true;
    }
    catch (svn::ClientException e)
    {
        StatusText::out(e.msg());
        return false;
    }
}


#include "svnclient.moc"
