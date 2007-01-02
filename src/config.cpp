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


//QSvn
#include "config.h"
#include "qsvn.h"

//Qt
#include <QtGui>


//make Config a singleton
Config* Config::m_instance = 0;

Config* Config::instance()
{
    if ( m_instance == 0 )
    {
        m_instance = new Config;
    }
    return m_instance;
}

//Config implementation
Config::Config( QObject *parent )
        : QObject( parent )
{
    QCoreApplication::setOrganizationName( "QSvn" );
    QCoreApplication::setOrganizationDomain( "" );
    QCoreApplication::setApplicationName( "QSvn" );
    _tempDir = QDir::tempPath() + QDir::separator() + "qsvn" + QDir::separator();
    _tempDir = QDir::convertSeparators( _tempDir );
}

void Config::removeTempDir()
{
    QDir tempDir( _tempDir );
    QStringListIterator it( tempDir.entryList() );
    while ( it.hasNext() )
        tempDir.remove( it.next() );

    tempDir.rmdir( _tempDir );
}

void Config::saveMainWindow( const QSvn *aQSvn )
{
    if ( aQSvn )
    {
        QSettings settings;

        saveWidget( aQSvn );

        saveSplitter( aQSvn, aQSvn->splitterVertical );
        saveSplitter( aQSvn, aQSvn->splitterHorizontal );
    }
}

void Config::restoreMainWindow( QSvn *aQSvn )
{
    if ( aQSvn )
    {
        QSettings settings;

        restoreWidget( aQSvn );

        //restore settings from splitterVertical
        restoreSplitter( aQSvn, aQSvn->splitterVertical );
        restoreSplitter( aQSvn, aQSvn->splitterHorizontal );
    }
}

void Config::saveWidget( const QWidget *widget, const QString prefix )
{
    QSettings settings;
    QString key = "widget" + prefix + widget->objectName();

    settings.remove( key );
    settings.setValue( key + "/geometry", widget->saveGeometry() );
}

void Config::restoreWidget( QWidget *widget, const QString prefix )
{
    QSettings settings;
    QString key = "widget" + prefix + widget->objectName();

    widget->restoreGeometry( settings.value( key + "/geometry" ).toByteArray( ) );
}

void Config::saveSplitter( const QObject *parent, const QSplitter *splitter )
{
    QSettings settings;
    QString key = parent->objectName() + "_" + splitter->objectName();

    settings.remove( key );
    settings.beginWriteArray( key, splitter->sizes().count() );
    for ( int i = 0; i < splitter->sizes().count(); ++i )
    {
        settings.setArrayIndex( i );
        settings.setValue( "value", splitter->sizes().at( i ) );
    }
    settings.endArray();
}

void Config::restoreSplitter( const QObject *parent, QSplitter *splitter )
{
    QSettings settings;
    QString key = parent->objectName() + "_" + splitter->objectName();
    int size = settings.beginReadArray( key );
    if ( size <= 0 )
        return;

    QList< int > list = splitter->sizes();
    for ( int i = 0; i < size; i++ )
    {
        settings.setArrayIndex( i );
        list.replace( i, settings.value( "value" ).toInt() );
    }
    settings.endArray();

    splitter->setSizes( list );
}

void Config::saveStringList( const QString &prefix, const QStringList &stringList )
{
    if ( prefix.isEmpty() )
        return;

    QSettings settings;

    settings.remove( prefix );
    settings.beginWriteArray( prefix, stringList.count() );
    for ( int i = 0; i < stringList.count(); ++i )
    {
        settings.setArrayIndex( i );
        settings.setValue( "entry" , stringList.at( i ) );
    }
    settings.endArray();
}

QStringList Config::getStringList( const QString &prefix )
{
    QStringList stringList;
    QSettings settings;

    int size = settings.beginReadArray( prefix );

    for ( int i = 0; i < size; ++i )
    {
        settings.setArrayIndex( i );
        stringList.append( settings.value( "entry" ).toString() );
    }
    settings.endArray();

    return stringList;
}

void Config::setValue( const QString &key, const QVariant &value )
{
    QSettings settings;
    settings.setValue( key, value );
}

QVariant Config::value( const QString &key )
{
    QSettings settings;
    return settings.value( key, defaultValue( key ) );
}

void Config::saveHeaderView( const QObject * parent, const QHeaderView * headerView )
{
    QSettings settings;
    QString key = parent->objectName() + "_" + headerView->parent()->objectName();

    settings.remove( key );
    settings.beginWriteArray( key, headerView->count() );
    for ( int i = 0; i < headerView->count(); ++i )
    {
        settings.setArrayIndex( i );
        settings.setValue( "value", headerView->sectionSize( i ) );
    }
    settings.endArray();
}

void Config::restoreHeaderView( const QObject * parent, QHeaderView * headerView )
{
    QSettings settings;
    QString key = parent->objectName() + "_" + headerView->parent()->objectName();

    settings.beginReadArray( key );
    for ( int i = 0; i < headerView->count(); i++ )
    {
        settings.setArrayIndex( i );
        headerView->resizeSection( i, settings.value( "value", headerView->sectionSize( i ) ).toInt() );
    }
    settings.endArray();
}

QString Config::tempDir()
{
    QDir dir( _tempDir );
    if ( !dir.exists() )
        dir.mkpath( _tempDir );
    return _tempDir;
}

QVariant Config::defaultValue( const QString & key )
{
    if ( key == KEY_SHOWLOGAFTERUPDATE )
        return true;
    else if ( key == KEY_CHECKEMPTYLOGMESSAGE )
        return true;
    else
        return QVariant();
}
