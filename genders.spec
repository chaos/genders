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

%prep
%setup  -q -n %{name}-%{version}

%build
./configure --prefix=/usr --with-xs-prefix="$RPM_BUILD_ROOT/usr" --with-genders-pm-prefix="$RPM_BUILD_ROOT/usr" --with-compat-installdir="/usr/lib/genders"
make 

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/man/man1
mkdir -p $RPM_BUILD_ROOT/usr/man/man3
mkdir -p $RPM_BUILD_ROOT/usr/lib/genders
DESTDIR="$RPM_BUILD_ROOT" make install 

%files
%defattr(-,root,root)
%doc README ChangeLog DISCLAIMER COPYING TUTORIAL genders.sample
/usr/man/man1/
/usr/man/man3/genders*
/usr/man/man3/libgenders.3.gz 
/usr/share/man/man3/
/usr/include/genders.h
/usr/lib/libgenders.*
/usr/bin/
/usr/lib/perl5/

%files compat
%defattr(-,root,root)
/usr/man/man3/gendlib*
/usr/lib/genders/


