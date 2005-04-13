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

#ifndef FILESELECTOR_H
#define FILESELECTOR_H

//QSvn
#include "fileselectordlg.h"


class FileSelector : public FileSelectorDlg
{
    Q_OBJECT

public:
    static FileSelector* Exemplar();
    static void releaseExemplar();

    void initFileSelector( int svnCommandType, const QString &path ); //!< set the parent and commandType

    void setSelectedFiles( QStringList* fileList );
    void setMessageString( QString message );

    QStringList* selectedFiles();
    QString messageString();

protected slots:
    void listViewFilesDoubleClickSlot();
    void buttonOkClickSlot();

private slots:
    void comboLogMessageActivatedSlot( const QString& selectedString );

private:
    FileSelector( QWidget *parent = 0, const char *name = 0 );
    ~FileSelector();

   static FileSelector* _exemplar;

   QString startPath;
   int _svnCommandType;
   int logMessageSize;
   void showLogMessage();
   void hideLogMessage();
};

#endif
