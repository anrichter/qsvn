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

#ifndef FILELIST_H
#define FILELIST_H

//Qt
#include <qobject.h>

//QSvn classes
class FileListItem;

//Qt classes
class QListView;
class QListViewItem;
class QString;


const int _COLUMN_STATUS = 1,
          _COLUMN_FILE = 0;
          
/**
This class handle the filelist of a directory from any working copy
 
@author Andreas Richter
*/

class FileList : public QObject
{
    Q_OBJECT
public:
    static FileList* Exemplar();
    static void releaseExemplar();

    QWidget *getWidget();
    FileListItem* selectedFileListItem(); //!< give the selected FileListItem
    QStringList* selectedFileListItems(); //!< give a QStringList with all selected FileNames

public slots:
    void updateListSlot( QString currentDirectory );
    void doubleClickedSlot( QListViewItem* item );

private:
    static FileList *_exemplar;
    QListView *listViewFiles;
    QString stringFullPath;

    FileList(QObject *parent = 0, const char *name = 0);
    ~FileList();
};

#endif
