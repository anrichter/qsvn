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

//qsvn
#include "config.h"

//Qt
#include <qsettings.h>
#include <qlistview.h>

//make Config a singleton
Config* Config::_exemplar = 0;

Config* Config::Exemplar()
{
    if ( _exemplar == 0 )
    {
        _exemplar = new Config;
    }
    return _exemplar;
}

//Config implementation
Config::Config(QObject *parent, const char *name)
        : QObject(parent, name)
{
    changed = FALSE;

    //read saved settings
    QSettings mySettings;
    mySettings.beginGroup( "qsvn/configuration" );
    _svnExecutable = mySettings.readEntry( "svnExecutable", "svn" );
    mySettings.endGroup();
}

Config::~Config()
{}

void Config::saveChanges()
{
    if ( changed )
    {
        //write the entire settings in config-file
        QSettings mySettings;
        mySettings.beginGroup( "qsvn/configuration" );
        mySettings.writeEntry( "svnExecutable", _svnExecutable );
        mySettings.endGroup();
    }
}

void Config::setSvnExecutable( QString aString )
{
    if ( aString != _svnExecutable )
    {
        _svnExecutable = aString;
        changed = TRUE;
    }
}

QString Config::getSvnExecutable()
{
    return _svnExecutable;
}

void Config::saveListView( QListView *aListView )
{
    if ( aListView )
    {
        QSettings mySettings;
        mySettings.beginGroup( "qsvn/listViews/" + QString( aListView->name() ) );
        for ( int i = 0; i < aListView->columns(); i++ )
        {
            mySettings.writeEntry( QString( "Column%1" ).arg( i ), aListView->columnWidth( i ) );
        }
        mySettings.endGroup();
    }
}

void Config::restoreListView( QListView *aListView )
{
    if ( aListView )
    {
        QSettings mySettings;
        mySettings.beginGroup( "qsvn/listViews/" + QString( aListView->name() ) );
        for ( int i = 0; i < aListView->columns(); i++ )
        {
            aListView->setColumnWidth( i, 
                                       mySettings.readNumEntry( QString( "Column%1" ).arg( i ), 
                                       aListView->columnWidth( i ) ) );
        }
        mySettings.endGroup();
    }
}
