/***************************************************************************
 *   This file is part of QSvn Project http://qsvn.berlios.de              *
 *   Copyright (c) 2004-2006 Andreas Richter <ar@oszine.de>                *
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

#ifndef CONFIG_H
#define CONFIG_H

//QSvn
class QSvn;

//Qt
#include <QObject>
class QSplitter;


/**
this singelton holds the configuration for qsvn

@author Andreas Richter
*/

class Config : public QObject
{
    Q_OBJECT
public:
    static Config* instance();

    void setDiffViewer( QString aString ); //!< set the complete path to a diff viewer like kompare
    QString getDiffViewer(); //!< get the complete path to a diff viewer

    //Main Window
    void saveMainWindow( QSvn *aQSvn );
    void restoreMainWindow( QSvn *aQSvn );

    /**
     * Save size and position from a widget.
     * @param widget The widget that was saved
     * @param prefix A prefix for unique identification additionally to the widget name.
     */
    void saveWidget( QWidget *widget, QString prefix = "" );
    /**
     * Restore saved size and position for a widget.
     * @param widget Widget that was restored.
     * @param prefix A prefix for unique identification additionally to the widget name.
     */
    void restoreWidget( QWidget *widget, QString prefix = "" );

    void saveSplitter( QObject *parent, QSplitter *splitter );
    void restoreSplitter( QObject *parent, QSplitter *splitter );

    void saveStringList( const QString &prefix, const QStringList &stringList );
    QStringList getStringList( const QString &prefix );

    void setValue( const QString &key, const QVariant &value );
    QVariant value( const QString &key );

    /*todo:
    //ListView Settings
    void saveListView( QListView *aListView );
    void restoreListView( QListView *aListView );
    */
public slots:

private:
    Config( QObject *parent = 0 );

    static Config* m_instance;

    QString _diffViewer;

    void saveChanges();
};

#endif
