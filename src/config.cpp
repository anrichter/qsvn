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

 
//QSvn
#include "config.h"
#include "qsvndlg.h"
#include "workingcopy.h"

//Qt
#include <qlistview.h>
#include <qsettings.h>
#include <qsplitter.h>


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

    QSettings settings;
    settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );

    //read saved settings
    settings.beginGroup( "configuration" );
    _svnExecutable = settings.readEntry( "svnExecutable", "svn" );
    _diffViewer = settings.readEntry( "diffViewer", "" );
    settings.endGroup();
}

Config::~Config()
{}

void Config::saveChanges()
{
    if ( changed )
    {
        //write the entire settings in config-file
        QSettings settings;
        settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );
        settings.beginGroup( "configuration" );
        settings.writeEntry( "svnExecutable", _svnExecutable );
        settings.writeEntry( "diffViewer", _diffViewer );
        settings.endGroup();
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

void Config::setDiffViewer( QString aString )
{
    if ( aString != _diffViewer )
    {
        _diffViewer = aString;
        changed = TRUE;
    }
}

QString Config::getDiffViewer()
{
    return _diffViewer;
}


void Config::saveMainWindow( QSvnDlg *aMainWindow )
{
    if ( aMainWindow )
    {
        QSettings settings;
        settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );
        settings.beginGroup( "mainwindow" );

        settings.writeEntry( "width", aMainWindow->width() );
        settings.writeEntry( "height", aMainWindow->height() );

        //save settings from splitterVertical
        int i = 0;
        QValueList<int> list = aMainWindow->splitterVertical->sizes();
        QValueList<int>::Iterator it = list.begin();
        while( it != list.end() )
        {
            settings.writeEntry( QString( "verticalSize%1" ).arg( i ), *it );
            ++it;
            ++i;
        }
        
        //save settings from splitterHorizontal
        i = 0;
        list = aMainWindow->splitterHorizontal->sizes();
        it = list.begin();
        while( it != list.end() )
        {
            settings.writeEntry( QString( "horizontalSize%1" ).arg( i ), *it );
            ++it;
            ++i;
        }
        
        settings.endGroup();
    }
}

void Config::restoreMainWindow( QSvnDlg *aMainWindow )
{
    if ( aMainWindow )
    {
        QSettings settings;
        settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );
        settings.beginGroup( "mainwindow" );
    
        aMainWindow->resize( settings.readNumEntry( "width", aMainWindow->width() ),
                            settings.readNumEntry( "height", aMainWindow->height() ) );
    
        //restore settings from splitterVertical
        int i = 0;
        QValueList<int> list = aMainWindow->splitterVertical->sizes();
        QValueList<int>::Iterator it = list.begin();
        while( it != list.end() )
        {
            *it = settings.readNumEntry( QString( "verticalSize%1" ).arg( i ), *it );
            ++it;
            ++i;
        }
        aMainWindow->splitterVertical->setSizes( list );   
    
        //restore settings from splitterHorizontal
        i = 0;
        list = aMainWindow->splitterHorizontal->sizes();
        it = list.begin();
        while( it != list.end() )
        {
            *it = settings.readNumEntry( QString( "horizontalSize%1" ).arg( i ), *it );
            ++it;
            ++i;
        }
        aMainWindow->splitterHorizontal->setSizes( list );   
        
        settings.endGroup();
    }
}

void Config::saveListView( QListView *aListView )
{
    if ( aListView )
    {
        QSettings settings;
        settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );
        settings.beginGroup( "listViews/" + QString( aListView->name() ) );
        for ( int i = 0; i < aListView->columns(); i++ )
        {
            settings.writeEntry( QString( "Column%1" ).arg( i ), aListView->columnWidth( i ) );
        }
        settings.endGroup();
    }
}

void Config::restoreListView( QListView *aListView )
{
    if ( aListView )
    {
        QSettings settings;
        settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );
        settings.beginGroup( "listViews/" + QString( aListView->name() ) );
        for ( int i = 0; i < aListView->columns(); i++ )
        {
            aListView->setColumnWidth( i,
                                       settings.readNumEntry( QString( "Column%1" ).arg( i ),
                                       aListView->columnWidth( i ) ) );
        }
        settings.endGroup();
    }
}

void Config::saveWorkingCopyEntries( QListView *aListView )
{
    if ( aListView )
    {
        QSettings settings;
        settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );
        
        int i = 0;
        QListViewItemIterator it( aListView );
        while ( it.current() )
        {
            if ( ! it.current()->parent() )
            {
                settings.beginGroup( "workingCopies/" +  QString( "WorkingCopy%1" ).arg( i ) );
                settings.writeEntry( "directory", it.current()->text( 0 ) );
                settings.endGroup();
                i++;
            }
            it++;
        }
        
        settings.beginGroup( "workingCopies/general" );
        settings.writeEntry( "count", i );
    }
}

void Config::restoreWorkingCopyEntries()
{
    QSettings settings;
    settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );
    settings.beginGroup( "workingCopies/general" );
    int count = settings.readNumEntry( "count", 0 );
    settings.endGroup();
    
    for ( int i = 0; i < count; i++ )
    {
        settings.beginGroup( "workingCopies/" + QString( "WorkingCopy%1" ).arg( i ) );
        WorkingCopy::Exemplar()->addExistingWorkingCopySlot( settings.readEntry( "directory", "" ) );
        settings.endGroup();
    }    
}
