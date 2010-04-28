/********************************************************************************
 *   This file is part of QSvn Project http://www.anrichter.net/projects/qsvn   *
 *   Copyright (c) 2004-2010 Andreas Richter <ar@anrichter.net>                 *
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

#ifndef CONFIG_H
#define CONFIG_H

//QSvn

class QSvn;

//Qt
#include <QObject>

class QHeaderView;

class QSettings;

class QSplitter;


//configuration strings
#define KEY_DIFFVIEWER "configuration/diffViewer"
#define KEY_SHOWLOGAFTERUPDATE "configuration/showLogAfterUpdate"
#define KEY_CHECKEMPTYLOGMESSAGE "configuration/checkEmptyLogMessage"
#define KEY_SHOWTEXTINTOOLBAR "configuration/checkShowTextInToolbar"
#define KEY_ICONSIZE "configuration/iconSize"
#define KEY_LASTWC "lastWC"

#define KEY_LASTMERGEWC "merge/%1_wc"
#define KEY_LASTMERGEFROMURL "merge/%1_fromUrl"
#define KEY_LASTMERGEFROMREVISION "merge/%1_fromRevision"
#define KEY_LASTMERGETOURL "merge/%1_toUrl"
#define KEY_LASTMERGETOREVISION "merge/%1_toRevision"

#define DEFAULT_ICONSIZE 22

/**
this singelton holds the configuration for qsvn

@author Andreas Richter
*/

class Config : public QObject
{
        Q_OBJECT

    public:
        static Config *instance();

        //Main Window
        void saveMainWindow(const QSvn *aQSvn);
        void restoreMainWindow(QSvn *aQSvn);

        /**
         * Save size and position from a widget.
         * @param widget The widget that was saved
         * @param prefix A prefix for unique identification additionally to the widget name.
         */
        void saveWidget(const QWidget *widget, const QString prefix = "");
        /**
         * Restore saved size and position for a widget.
         * @param widget Widget that was restored.
         * @param prefix A prefix for unique identification additionally to the widget name.
         */
        void restoreWidget(QWidget *widget, const QString prefix = "");

        void saveSplitter(const QObject *parent, const QSplitter *splitter);
        void restoreSplitter(const QObject *parent, QSplitter *splitter);

        void saveStringList(const QString &prefix, const QStringList &stringList);
        QStringList getStringList(const QString &prefix);

        void setValue(const QString &key, const QVariant &value);
        QVariant value(const QString &key);
        QVariant value(const QString &key, const QVariant &defaultValue);

        void saveHeaderView(const QObject *parent, const QHeaderView *headerView);
        void restoreHeaderView(const QObject *parent, QHeaderView *headerView);

        QString tempDir();
        void removeTempDir();

        void setIniFile(const QString iniFile);

    private:
        Config(QObject *parent = 0);

        static Config *m_instance;
        QSettings *m_settings;
        QString m_tempDir;


        QVariant defaultValue(const QString &key); //returns default values for configuration parameters
};

#endif
