# norootforbuild

Name:      qsvn
License:   GPL
Group:     Development/Tools/Version Control
Summary:   GUI Subversion Client
Vendor:    Andreas Richter <ar@anrichter.net>
Url:       http://www.anrichter.net/projects/qsvn/
BuildRoot: %{_tmppath}/%{name}-root
Version:   0.8.90
Release:   1%{?dist}
Source:    qsvn-%version-src.tar.gz

Requires: subversion >= 1.3.0
Requires: libqt4
BuildRequires: gcc-c++
BuildRequires: cmake >= 2.4
BuildRequires: subversion-devel >= 1.3.0
BuildRequires: libqt4-devel

%if 0%{?suse_version} > 0
Requires: libapr1
Requires: libapr-util1
BuildRequires: xorg-x11-devel
BuildRequires: libapr1-devel
BuildRequires: libapr-util1-devel
BuildRequires: update-desktop-files
%define _prefix /usr
%endif

%if 0%{?fedora_version} > 0
Requires: apr
Requires: apr-util
BuildRequires: apr-devel
BuildRequires: apr-util-devel
%define _prefix /usr  
%endif  

%description
QSvn is a GUI Subversion client for Linux, UNIX, Mac OS X and Windows. 
It is a real client not a GUI wrapper for the command line client from Subversion.

%prep
%setup -q
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE="Release" ../src

%build
cd build
make

%install
cd build
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
rm -rf %{buildroot}%{_includedir}/svnqt/
rm -rf %{buildroot}/%{_datadir}/svnqt/*
install -d -m 0755 %{buildroot}%{_datadir}/pixmaps
install -m 0644  ../src/images/%{name}.png  %{buildroot}%{_datadir}/pixmaps
install -d -m 0755 %{buildroot}%{_datadir}/applications
install -m 0644  ../src/%{name}.desktop  %{buildroot}%{_datadir}/applications

%if 0%{?suse_version}  
%suse_update_desktop_file %{name}  
%endif

%post
/sbin/ldconfig > /dev/null 2>&1

%postun
/sbin/ldconfig > /dev/null 2>&1

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_bindir}/*
%{_libdir}/lib*
%{_datadir}/applications/*.desktop
%{_datadir}/pixmaps/*.png

%changelog
