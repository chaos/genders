Summary: Static cluster configuration database and tools.
Name: genders
Version: 0.1
Release: 1
Copyright: none
Group: System Environment/Base
Source: genders-0.1.tgz
BuildRoot: /var/tmp/%{name}-buildroot
Prereq: rdist pdksh net-tools diffutils perl

%description
Genders is a static cluster configuration database and tools for querying
and building rdist Distfile macros.

%prep
%setup

%build
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/man/man1
mkdir -p $RPM_BUILD_ROOT/usr/man/man3
install -m 755 -d $RPM_BUILD_ROOT/usr/lib/genders
install -m 755 gendlib.pl $RPM_BUILD_ROOT/usr/lib/genders
install -m 755 hostlist.pl $RPM_BUILD_ROOT/usr/lib/genders
install -s -m 755 inst $RPM_BUILD_ROOT/usr/bin
install -m 755 dist2 $RPM_BUILD_ROOT/usr/bin
install -m 755 nodeattr $RPM_BUILD_ROOT/usr/bin
install -m 755 dist_all $RPM_BUILD_ROOT/usr/bin
install -m 755 dist_local $RPM_BUILD_ROOT/usr/bin
install -m 755 dist_cmp $RPM_BUILD_ROOT/usr/bin
install -m 755 dist_bootstrap $RPM_BUILD_ROOT/usr/bin
gzip dist2.1 dist_all.1 dist_cmp.1 dist_local.1 inst.1 nodeattr.1 gendlib.3
install -m 644 dist2.1.gz $RPM_BUILD_ROOT/usr/man/man1
install -m 644 dist_all.1.gz $RPM_BUILD_ROOT/usr/man/man1
install -m 644 dist_cmp.1.gz $RPM_BUILD_ROOT/usr/man/man1
install -m 644 dist_local.1.gz $RPM_BUILD_ROOT/usr/man/man1
install -m 644 inst.1.gz $RPM_BUILD_ROOT/usr/man/man1
install -m 644 nodeattr.1.gz $RPM_BUILD_ROOT/usr/man/man1
install -m 644 gendlib.3.gz $RPM_BUILD_ROOT/usr/man/man3

%files
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
/usr/man/man1/dist2.1.gz
/usr/man/man1/dist_all.1.gz
/usr/man/man1/dist_cmp.1.gz
/usr/man/man1/dist_local.1.gz
/usr/man/man1/inst.1.gz
/usr/man/man1/nodeattr.1.gz
/usr/man/man3/gendlib.3.gz

%changelog
