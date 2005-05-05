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


//QSvnTest
#include "svnclienttests.h"

//QSvn
#include "../svnclient.h"

//Qt
#include <qdir.h>
#include <qstring.h>
#include <qstringlist.h>

CppUnit::Test *SvnClientTests::testSuite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "SvnClientTests" );
    suiteOfTests->addTest( new CppUnit::TestCaller<SvnClientTests>( 
                                   "testCheckout", 
                                   &SvnClientTests::testCheckout ) );
    return suiteOfTests;
}

void SvnClientTests::setUp()
{
    //remove old temporary working copy
    rm_rf( QString( TEST_WC_DIR ) + QDir::separator() + QString( TEST_REPO ) );
}

void SvnClientTests::tearDown()
{
    //remove temporary working copy
    rm_rf( QString( TEST_WC_DIR ) + QDir::separator() + QString( TEST_REPO ) );
}

void SvnClientTests::testCheckout()
{
    SvnClient::Exemplar()->checkout( QString( TEST_REPO_FILE_URL ) + "/" + QString( TEST_REPO ), 
                                     QString( TEST_WC_DIR ) );
    QString svndir = QString( TEST_WC_DIR ) + QDir::separator() + ".svn" + QDir::separator();
    CPPUNIT_ASSERT( QFile::exists( svndir + "entries" ) && 
                    QFile::exists( svndir + "format" ) && 
                    QFile::exists( svndir + "README" ) );
}

void SvnClientTests::rm_rf( const QString &directory )
{
    QDir dir( directory );
    if ( !dir.exists() ) 
    {
        return;
    }
    
    //search all directories and call rm_rf recursively
    QStringList entries = dir.entryList( QDir::Dirs, QDir::Name );
    for ( QStringList::Iterator it = entries.begin(); it != entries.end(); ++it )
    {
        if ( ( *it != "." ) && ( *it != ".." ) )
        {
            rm_rf( directory + QDir::separator() + *it );
        }
    }
    
    //delete all files in current directory
    QStringList files = dir.entryList( QDir::Files, QDir::Name );
    for (QStringList::Iterator it = files.begin(); it != files.end(); ++it )
    {
        dir.remove( directory + QDir::separator() + *it );
    }
    
    //delete current directory itself
    dir.rmdir( directory );
}
