Name: 
Version:
Release:
Summary: Static cluster configuration database and tools.
Copyright: none
Group: System Environment/Base
Source: %{name}-%{version}-%{release}.tgz
BuildRoot: /var/tmp/%{name}-buildroot
Prereq: rdist pdksh net-tools diffutils perl

%description
Genders is a static cluster configuration database and tools for querying
and building rdist Distfile macros.

%prep
%setup  -n %{name}-%{version}-%{release}

%build
make all

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/man/man1
mkdir -p $RPM_BUILD_ROOT/usr/man/man3
install -d $RPM_BUILD_ROOT/usr/lib/genders
install gendlib.pl $RPM_BUILD_ROOT/usr/lib/genders
install hostlist.pl $RPM_BUILD_ROOT/usr/lib/genders
install -s inst $RPM_BUILD_ROOT/usr/bin
install dist2 $RPM_BUILD_ROOT/usr/bin
install nodeattr $RPM_BUILD_ROOT/usr/bin
install dist_all $RPM_BUILD_ROOT/usr/bin
install dist_local $RPM_BUILD_ROOT/usr/bin
install dist_cmp $RPM_BUILD_ROOT/usr/bin
install dist_bootstrap $RPM_BUILD_ROOT/usr/bin
install updatefstab $RPM_BUILD_ROOT/usr/bin
install updateservices $RPM_BUILD_ROOT/usr/bin
gzip dist2.1 dist_all.1 dist_cmp.1 dist_local.1 inst.1 nodeattr.1 gendlib.3
install dist2.1.gz $RPM_BUILD_ROOT/usr/man/man1
install dist_all.1.gz $RPM_BUILD_ROOT/usr/man/man1
install dist_cmp.1.gz $RPM_BUILD_ROOT/usr/man/man1
install dist_local.1.gz $RPM_BUILD_ROOT/usr/man/man1
install inst.1.gz $RPM_BUILD_ROOT/usr/man/man1
install nodeattr.1.gz $RPM_BUILD_ROOT/usr/man/man1
install gendlib.3.gz $RPM_BUILD_ROOT/usr/man/man3

%files
%defattr(-,root,root)
%doc README ChangeLog DISCLAIMER genders.sample clusters.sample Distfile.sample
/usr/lib/genders/gendlib.pl
/usr/lib/genders/hostlist.pl
/usr/bin/inst
/usr/bin/dist2
/usr/bin/nodeattr
/usr/bin/dist_all
/usr/bin/dist_local
/usr/bin/dist_cmp
/usr/bin/dist_bootstrap
/usr/bin/updatefstab
/usr/bin/updateservices
/usr/man/man1/dist2.1.gz
/usr/man/man1/dist_all.1.gz
/usr/man/man1/dist_cmp.1.gz
/usr/man/man1/dist_local.1.gz
/usr/man/man1/inst.1.gz
/usr/man/man1/nodeattr.1.gz
/usr/man/man3/gendlib.3.gz
