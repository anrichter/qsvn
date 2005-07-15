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


//QSvn
#include "Config.h"

//Qt
#include <QCoreApplication>
#include <QSettings>


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
Config::Config( QObject *parent )
        : QObject( parent )
{
    QCoreApplication::setOrganizationName( "QSvn" );
    QCoreApplication::setOrganizationDomain( "" );
    QCoreApplication::setApplicationName( "QSVN" );

    QSettings settings;

    //read saved settings
    _svnExecutable = settings.value( "configuration/svnExecutable", "svn" ).toString();
    _diffViewer = settings.value( "configuration/diffViewer", "" ).toString();
}

void Config::saveChanges()
{
    //write the entire settings in config-file
    QSettings settings;
    settings.setValue( "configuration/svnExecutable", _svnExecutable );
    settings.setValue( "configuration/diffViewer", _diffViewer );
}

void Config::setSvnExecutable( QString aString )
{
    if ( aString != _svnExecutable )
    {
        _svnExecutable = aString;
        saveChanges();
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
        saveChanges();
    }
}

QString Config::getDiffViewer()
{
    return _diffViewer;
}

/*todo:
void Config::saveMainWindow( QSvnDlg *aMainWindow )
{
    if ( aMainWindow )
    {
        QSettings settings;
        settings.setPath( _SETTINGS_DOMAIN, _SETTINGS_PRODUCT, QSettings::User );
        settings.beginGroup( "mainwindow" );

        settings.writeEntry( "width", aMainWindow->width() );
        settings.writeEntry( "height", aMainWindow->height() );
        settings.writeEntry( "x", aMainWindow->x() );
        settings.writeEntry( "y", aMainWindow->y() );

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
        
        aMainWindow->move( QPoint( settings.readNumEntry( "x", aMainWindow->x() ),
                                   settings.readNumEntry( "y", aMainWindow->y() ) ) );


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
*/
