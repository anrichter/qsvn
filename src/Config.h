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

#ifndef CONFIG_H
#define CONFIG_H

//QSvn
class QSvn;

//Qt
#include <QObject>


/**
this singelton holds the configuration for qsvn

@author Andreas Richter
*/

class Config : public QObject
{
    Q_OBJECT
public:
    static Config* Exemplar();

    void setSvnExecutable( QString aString ); //!< set the complete path to svn execute
    QString getSvnExecutable(); //!< get the complete path to svn execute

    void setDiffViewer( QString aString ); //!< set the complete path to a diff viewer like kompare
    QString getDiffViewer(); //!< get the complete path to a diff viewer

    //Main Window
    void saveMainWindow( QSvn *aQSvn );
    void restoreMainWindow( QSvn *aQSvn );

    /*todo:
    //ListView Settings
    void saveListView( QListView *aListView );
    void restoreListView( QListView *aListView );

    //Working Copys
    void saveWorkingCopyEntries( QListView *aListView );
    void restoreWorkingCopyEntries();
    */
public slots:

private:
    Config( QObject *parent = 0 );

    static Config* _exemplar;

    QString _svnExecutable;
    QString _diffViewer;

    void saveChanges();
};

#endif
