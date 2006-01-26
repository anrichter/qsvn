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

#ifndef ADDWORKINGCOPY_H
#define ADDWORKINGCOPY_H

//QSvn
#include "ui_addworkingcopy.h"


/**
This dialog let the user choice an existing working copy for add to the working copy list.

@author Andreas Richter
 */

class AddWorkingCopy : public QDialog, public Ui::AddWorkingCopy
{
    Q_OBJECT

public:
    AddWorkingCopy( QWidget *parent = 0 );
    ~AddWorkingCopy();

    /**
     * Get the selected directory.
     * @return selected directory.
     */
    QString getSelectedDirectory() const;
public slots:

private:

protected slots:
    void selectDirectorySlot();
};

#endif
