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

#ifndef SSLSERVERTRUST_H
#define SSLSERVERTRUST_H

//QSvn
#include "ui_sslservertrust.h"

//SvnCpp
#include "svnqt/context_listener.hpp"

//Qt
#include <QDialog>


class SslServerTrust : public QDialog, public Ui::SslServerTrust
{
        Q_OBJECT

    public:
        SslServerTrust(QWidget *parent = 0, const svn::ContextListener::SslServerTrustData &data = 0);

        static svn::ContextListener::SslServerTrustAnswer getSslServerTrustAnswer(const svn::ContextListener::SslServerTrustData &data);
        svn::ContextListener::SslServerTrustAnswer answer();

    private:
        svn::ContextListener::SslServerTrustAnswer m_answer;

    private slots:
        void doAcceptTemporarily();
        void doAcceptPermanently();
};

#endif
