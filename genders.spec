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

%package compat
Summary: compatability library 
Group: System Environment/Base
%description compat
genders API that is compatible with earlier releases of genders

%package dist
Summary: genders tools
Group: System Environment/Base
%description dist
set of tools distributed with genders

%prep
%setup  -q -n %{name}-%{version}

%build
./configure --prefix=/usr --with-xs-prefix="$RPM_BUILD_ROOT/usr" --with-genders-pm-prefix="$RPM_BUILD_ROOT/usr"
make 

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/man/man1
mkdir -p $RPM_BUILD_ROOT/usr/man/man3
mkdir -p $RPM_BUILD_ROOT/usr/share/man/man3
install -d $RPM_BUILD_ROOT/usr/lib/genders
install compat/gendlib.pl $RPM_BUILD_ROOT/usr/lib/genders
install compat/hostlist.pl $RPM_BUILD_ROOT/usr/lib/genders
install dist/dist2 $RPM_BUILD_ROOT/usr/bin
install dist/dist_all $RPM_BUILD_ROOT/usr/bin
install dist/dist_local $RPM_BUILD_ROOT/usr/bin
install dist/dist_cmp $RPM_BUILD_ROOT/usr/bin
install dist/dist_bootstrap $RPM_BUILD_ROOT/usr/bin
DESTDIR="$RPM_BUILD_ROOT" make install 

%files
%defattr(-,root,root)
%doc README ChangeLog DISCLAIMER COPYING
/usr/man/man3/genders*
/usr/man/man3/libgenders.3.gz 
/usr/share/man/man3/Libgenders.3pm.gz
/usr/include/genders.h
/usr/lib/libgenders.*
/usr/lib/perl5/

%files compat
%defattr(-,root,root)
%doc doc/TUTORIAL doc/genders.sample doc/clusters.sample doc/Distfile.sample
/usr/man/man3/gendlib*
/usr/lib/genders/gendlib.pl
/usr/lib/genders/hostlist.pl

%files dist
%defattr(-,root,root)
/usr/man/man1/*
/usr/bin/*

