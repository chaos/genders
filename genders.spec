Name: 
Version:
Release:
Summary: Static cluster configuration database.
Copyright: none
Group: System Environment/Base
Source0:
Prereq: rdist pdksh net-tools diffutils perl
BuildRoot: %{_tmppath}/%{name}-%{version}

%description
Genders is a static cluster configuration database.  

%package dist
Summary: genders tools
Group: System Environment/Base
%description dist
set of tools distributed with genders

%package compat
Summary: compatability library 
Group: System Environment/Base
%description compat
genders API that is compatible with earlier releases of genders

%prep
%setup  -q -n %{name}-%{version}

%build
./configure --prefix=/usr --with-xs-prefix="$RPM_BUILD_ROOT/usr" --with-genders-pm-prefix="$RPM_BUILD_ROOT/usr" --with-compat-installdir="$RPM_BUILD_ROOT/usr/lib/genders"
make 

%install
rm -rf $RPM_BUILD_ROOT
DESTDIR="$RPM_BUILD_ROOT" make install 

%files
%defattr(-,root,root)
%doc README ChangeLog DISCLAIMER COPYING
/usr/man/man3/genders*
/usr/man/man3/libgenders.3.gz 
/usr/share/man/man3/*
/usr/include/genders.h
/usr/lib/libgenders.*
/usr/bin/nodeattr
/usr/lib/perl5/

%files dist
%defattr(-,root,root)
%doc doc/TUTORIAL doc/genders.sample doc/clusters.sample doc/Distfile.sample
/usr/man/man1/*
/usr/bin/dist*
/usr/bin/inst

%files compat
%defattr(-,root,root)
/usr/man/man3/gendlib*
/usr/lib/genders/gendlib.pl
/usr/lib/genders/hostlist.pl


