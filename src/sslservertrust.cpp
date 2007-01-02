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
#include "sslservertrust.h"

//SvnCpp
#include "svnqt/context_listener.hpp"

//Qt
#include <QtCore>


SslServerTrust::SslServerTrust( QWidget * parent, const svn::ContextListener::SslServerTrustData & data  )
        : QDialog( parent )
{
    setupUi( this );

    m_answer = svn::ContextListener::DONT_ACCEPT;

    QString infoText;
    infoText.append( "Hostname: " + data.hostname + "<br />" );
    infoText.append( "Fingerprint: " + data.fingerprint + "<br />" );
    infoText.append( "valid From: " + data.validFrom + "<br />" );
    infoText.append( "valid Until: " + data.validUntil + "<br />" );
    infoText.append( "issuer DName: " + data.issuerDName + "<br />" );
    infoText.append( "Realm: " + data.realm );
    labelInformation->setText( infoText );

    connect( buttonAcceptTemporarily, SIGNAL( clicked( bool ) ), this, SLOT( doAcceptTemporarily() ) );
    connect( buttonAcceptPermanently, SIGNAL( clicked( bool ) ), this, SLOT( doAcceptPermanently() ) );
}

void SslServerTrust::doAcceptTemporarily( )
{
    m_answer = svn::ContextListener::ACCEPT_TEMPORARILY;
    this->accept();
}

void SslServerTrust::doAcceptPermanently( )
{
    m_answer = svn::ContextListener::ACCEPT_PERMANENTLY;
    this->accept();
}

svn::ContextListener::SslServerTrustAnswer SslServerTrust::answer( )
{
    return m_answer;
}

svn::ContextListener::SslServerTrustAnswer SslServerTrust::getSslServerTrustAnswer( const svn::ContextListener::SslServerTrustData & data )
{
    SslServerTrust sslServerTrust( 0, data );
    sslServerTrust.exec();
    return sslServerTrust.answer();
}

#include "sslservertrust.moc"
