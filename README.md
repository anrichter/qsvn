# About

QSvn is a graphical Subversion client for Linux, Windows and Mac OS X.

QSvn is released under the terms of the GNU GENERAL PUBLIC LICENSE Version 2, June 1991.
The License you found in the file licenses/COPYING.
Parts of the binary distribution are released under different license. You found all of them in the licenses folder.

More Information about QSvn you will find at http://anrichter.github.io/qsvn/

# End of Lifetime

The development of QSvn was stopped at the end of 2010.
But feel free to clone the repository and go on working on the next release of QSvn.

# Checkout the Sources

QSvn uses the [svnqt](https://github.com/MIRAvzw/svnqt) library to access Subversion repositories.
So the checkout of the QSvn sources alone is not enough.
You have to checkout svnqt sources also.

## The git way

    git clone https://github.com/anrichter/qsvn
    cd qsvn
    git submodule init
    git submodule update

## The Subversion way

    svn checkout https://github.com/anrichter/qsvn/trunk qsvn
    cd qsvn/src
    svn checkout https://github.com/MIRAvzw/svnqt/trunk@340 svnqt


# Compile & Install under UNIX

For compilation under UNIX you need the following software packages:

- Qt4
- cmake 2.4.0 or greater
- subversion libraries and headers

## Debian

Prepare system for build with debugger and cmake:

    sudo apt-get install build-essential gdb cmake

Install Qt4

    sudo apt-get install libqt4-core libqt4-dbg libqt4-dev libqt4-gui \
    libqt4-qt3support libqt4-sql qt4-designer qt4-dev-tools qt4-doc \
    qt4-qtconfig

Install Subversion and Libraries

    sudo apt-get install subversion libsvn-dev libapr1 libapr1-dbg \
    libapr1-dev libaprutil1 libaprutil1-dbg libaprutil1-dev

It is recommended to use ccache for increase continuous builds

    sudo apt-get install ccache
    add "export PATH="/usr/lib/ccache:$PATH" to ~/.bashrc

If you like to develop under KDE i recommended to

    sudo apt-get install kdevelop kde-devel-extras

## Compile for Release

    mkdir build
    cd build
    ../scripts/release.sh

When you would install qsvn call

    sudo make install

## Compile for Developing

    mkdir build
    cd build
    cmake -D CMAKE_BUILD_TYPE="Debug" ../src
    make

If you prefer to use KDevelop as an IDE type:

    mkdir build
    cd build
    cmake -G "KDevelop3" -D CMAKE_BUILD_TYPE="Debug" ../src


# Compile & Install under Windows

For compilation under Windows you need the following software packages:

- Microsoft Visual Studio 2008
  I recommended the free available Express Edition from
    http://www.microsoft.com/express/download/
  After installation of Microsoft Visual Studio 2008 you can open a Visual Studio
  2008 Command prompt to compile source code with the Visual C++ Compiler.
- Subversion & Development Libraries
  Download the latest Subversion Release and the corresponding Development Libraries.
  Install Subversion in your Program Folder.
  Unpack the development libraries into the Subversion installation folder.
    http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=91
  In addition to this you need the libintl development libraries and headers
    http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=2627
  and the Berkley DB libraries and headers
    http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=688
- CMake 2.4.4 or greater
    http://www.cmake.org
- Qt4 compiled for use with MSVC++
  Download the latest Qt4 Source Package for Windows.
    http://www.trolltech.com
  After the Qt is compiled you must add the Qt-bin-directory to your PATH variable.
  CMake is looking for Qt libraries in PATH-directories also.
- InnoSetup with QuickStart Pack (http://www.jrsoftware.org/isinfo.php)
  In order to build the installation you need to install Install InnoSetup
  and the Preprocessor. After you had install InnoSetup, add it to your PATH.


## Compile for Release

    mkdir release
    cd release
    c:\path\to\qsvn\source\scripts\release.cmd -D SUBVERSION_INSTALL_PATH=c:\path\to\svn


## Compile for Development

    mkdir build
    cd build
    cmake -G "NMake Makefiles" -D CMAKE_BUILD_TYPE="Debug" ..\src
    nmake

If you prefer the Microsoft Visual C++ 2005 Express Edition as your IDE type in:

    mkdir build
    cd build
    cmake -G "Visual Studio 9 2008" ..\src

## Build Installation

For build the QSvn Installer for Windows follow the instruction found in
  src\setup\README
