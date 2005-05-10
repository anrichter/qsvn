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
#include "svnclientsvntests.h"

//QSvn
#include "../svnclient.h"

//Qt
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qprocess.h>
#include <qstring.h>
#include <qstringlist.h>

//Std
#ifdef Q_WS_X11
#include <unistd.h>
#endif

#ifdef Q_WS_WIN
#include <windows.h>
#endif


CppUnit::Test *SvnClientSvnTests::testSuite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "SvnClientSvnTests" );
    suiteOfTests->addTest( new CppUnit::TestCaller<SvnClientSvnTests>( 
                                   "testCheckout", 
                                   &SvnClientSvnTests::testCheckout ) );
    return suiteOfTests;
}

void SvnClientSvnTests::setUp()
{
    //remove old temporary working copy
    rm_rf( TEST_WC_DIR + QDir::separator() + TEST_REPO );
}

void SvnClientSvnTests::tearDown()
{
    //remove temporary working copy
    rm_rf( TEST_WC_DIR + QDir::separator() + TEST_REPO );
}

void SvnClientSvnTests::testCheckout()
{
    SvnClient::Exemplar()->checkout( TEST_REPO_SVN_URL + "/" + TEST_REPO, 
                                     TEST_WC_DIR );

    QString svndir = TEST_WC_DIR + QDir::separator() + 
                     TEST_REPO + QDir::separator() + 
                     ".svn" + QDir::separator();
    
	CPPUNIT_ASSERT( QFile::exists( svndir + "entries" ) && 
                    QFile::exists( svndir + "format" ) && 
                    QFile::exists( svndir + "README.txt" ) );
}

void SvnClientSvnTests::rm_rf( const QString &directory )
{
    QFileInfo fileInfo( directory );
    if ( fileInfo.isDir() )
    {
        QProcess *proc = new QProcess();
    
#ifdef Q_WS_X11
        proc->addArgument( "rm" );
        proc->addArgument( "-rf" );
        proc->addArgument( directory );
#endif
#ifdef Q_WS_WIN
        proc->addArgument( "cmd" );
        proc->addArgument( "/C" );
        proc->addArgument( "rmdir" );
        proc->addArgument( "/Q" );
        proc->addArgument( "/S" );
        proc->addArgument( directory );
#endif

        proc->start();
        while ( proc->isRunning() )
        {
#ifdef Q_WS_X11
            sleep( 1 );
#endif

#ifdef Q_WS_WIN
            Sleep( 1 );
#endif
        }
    }
}
