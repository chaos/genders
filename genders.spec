Name: 
Version:
Release:
Summary: Static cluster configuration database and tools.
Copyright: none
Group: System Environment/Base
Source0:
Prereq: rdist pdksh net-tools diffutils perl
BuildRoot: %{_tmppath}/%{name}-%{version}

%description
Genders is a static cluster configuration database and tools for querying
and building rdist Distfile macros.

%prep
%setup  -q -n %{name}-%{version}

%build
make all

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/man/man1
mkdir -p $RPM_BUILD_ROOT/usr/man/man3
mkdir -p $RPM_BUILD_ROOT/usr/lib/genders/auto/Libgenders
install -d $RPM_BUILD_ROOT/usr/lib/genders
install gendlib.pl $RPM_BUILD_ROOT/usr/lib/genders
install hostlist.pl $RPM_BUILD_ROOT/usr/lib/genders
install -s inst $RPM_BUILD_ROOT/usr/bin
install dist2 $RPM_BUILD_ROOT/usr/bin
install dist_all $RPM_BUILD_ROOT/usr/bin
install dist_local $RPM_BUILD_ROOT/usr/bin
install dist_cmp $RPM_BUILD_ROOT/usr/bin
install dist_bootstrap $RPM_BUILD_ROOT/usr/bin
gzip dist2.1 dist_all.1 dist_cmp.1 dist_local.1 inst.1 nodeattr.1 gendlib.3
install dist2.1.gz $RPM_BUILD_ROOT/usr/man/man1
install dist_all.1.gz $RPM_BUILD_ROOT/usr/man/man1
install dist_cmp.1.gz $RPM_BUILD_ROOT/usr/man/man1
install dist_local.1.gz $RPM_BUILD_ROOT/usr/man/man1
install inst.1.gz $RPM_BUILD_ROOT/usr/man/man1
install nodeattr.1.gz $RPM_BUILD_ROOT/usr/man/man1
install gendlib.3.gz $RPM_BUILD_ROOT/usr/man/man3
cd src/libgenders/man; gzip genders_attrlist_clear.3 genders_attrlist_create.3 genders_attrlist_destroy.3 genders_close.3 genders_errnum.3 genders_errormsg.3 genders_getattr.3 genders_getattr_all.3 genders_getmaxattrlen.3 genders_getmaxattrs.3 genders_getmaxnodelen.3 genders_getmaxvallen.3 genders_getnodename.3 genders_getnodes.3 genders_getnumattrs.3 genders_getnumnodes.3 genders_handle_create.3 genders_handle_destroy.3 genders_handle_dump.3 genders_nodelist_clear.3 genders_nodelist_create.3 genders_nodelist_destroy.3 genders_open.3 genders_perror.3 genders_strerror.3 genders_testattr.3 genders_testattrval.3 genders_testnode.3 genders_vallist_clear.3 genders_vallist_create.3 genders_vallist_destroy.3 genders_list.3 genders_stats.3 genders_errors.3 genders_parse.3 libgenders.3 Libgenders.3; cd ../../..
install src/libgenders/man/genders_attrlist_clear.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_attrlist_create.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_attrlist_destroy.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_close.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_errormsg.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_errnum.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getattr.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getattr_all.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getmaxattrlen.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getmaxattrs.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getmaxnodelen.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getmaxvallen.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getnodename.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getnodes.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getnumattrs.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_getnumnodes.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_handle_create.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_handle_destroy.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_handle_dump.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_nodelist_clear.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_nodelist_create.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_nodelist_destroy.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_open.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_perror.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_strerror.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_testattr.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_testattrval.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_testnode.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_vallist_clear.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_vallist_create.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_vallist_destroy.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_list.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_stats.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_errors.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/genders_parse.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/libgenders.3.gz $RPM_BUILD_ROOT/usr/man/man3
install src/libgenders/man/Libgenders.3.gz $RPM_BUILD_ROOT/usr/man/man3
cp src/libgenders/genders.h src/libgenders/Libgenders	
cp src/libgenders/genders.c src/libgenders/Libgenders	
cd src/libgenders/Libgenders; perl Makefile.PL; make; cd ../../..
install src/libgenders/Libgenders/blib/arch/auto/Libgenders/Libgenders.so $RPM_BUILD_ROOT/usr/lib/genders/auto/Libgenders
install src/libgenders/Libgenders/blib/arch/auto/Libgenders/Libgenders.bs $RPM_BUILD_ROOT/usr/lib/genders/auto/Libgenders
install src/libgenders/Libgenders/blib/lib/Libgenders.pm $RPM_BUILD_ROOT/usr/lib/genders
install src/libgenders/Libgenders/blib/lib/auto/Libgenders/autosplit.ix $RPM_BUILD_ROOT/usr/lib/genders/auto/Libgenders
cd src; DESTDIR="$RPM_BUILD_ROOT" make install 

%files
%defattr(-,root,root)
%doc README TUTORIAL ChangeLog DISCLAIMER COPYING
%doc genders.sample clusters.sample Distfile.sample
/usr/lib/genders/gendlib.pl
/usr/lib/genders/hostlist.pl
/usr/lib/genders/Libgenders.pm
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
/usr/man/man3/genders_open.3.gz
/usr/man/man3/genders_handle_create.3.gz
/usr/man/man3/genders_handle_destroy.3.gz
/usr/man/man3/genders_close.3.gz
/usr/man/man3/genders_errnum.3.gz
/usr/man/man3/genders_errormsg.3.gz
/usr/man/man3/genders_strerror.3.gz
/usr/man/man3/genders_perror.3.gz
/usr/man/man3/genders_handle_dump.3.gz
/usr/man/man3/genders_getnumnodes.3.gz
/usr/man/man3/genders_getnumattrs.3.gz
/usr/man/man3/genders_getmaxattrs.3.gz
/usr/man/man3/genders_getmaxnodelen.3.gz
/usr/man/man3/genders_getmaxattrlen.3.gz
/usr/man/man3/genders_getmaxvallen.3.gz
/usr/man/man3/genders_nodelist_create.3.gz
/usr/man/man3/genders_nodelist_clear.3.gz
/usr/man/man3/genders_nodelist_destroy.3.gz
/usr/man/man3/genders_attrlist_create.3.gz
/usr/man/man3/genders_attrlist_clear.3.gz
/usr/man/man3/genders_attrlist_destroy.3.gz
/usr/man/man3/genders_vallist_create.3.gz
/usr/man/man3/genders_vallist_clear.3.gz
/usr/man/man3/genders_vallist_destroy.3.gz
/usr/man/man3/genders_getnodename.3.gz
/usr/man/man3/genders_getnodes.3.gz
/usr/man/man3/genders_getattr.3.gz
/usr/man/man3/genders_getattr_all.3.gz
/usr/man/man3/genders_testattr.3.gz
/usr/man/man3/genders_testattrval.3.gz
/usr/man/man3/genders_testnode.3.gz
/usr/man/man3/genders_list.3.gz
/usr/man/man3/genders_stats.3.gz
/usr/man/man3/genders_errors.3.gz
/usr/man/man3/genders_parse.3.gz
/usr/man/man3/libgenders.3.gz
/usr/man/man3/Libgenders.3.gz
/usr/lib/libgenders.a
/usr/lib/libgenders.la
/usr/lib/libgenders.so
/usr/lib/libgenders.so.0
/usr/lib/libgenders.so.0.0.0
/usr/include/genders.h
/usr/lib/genders/auto/Libgenders/Libgenders.bs
/usr/lib/genders/auto/Libgenders/Libgenders.so
/usr/lib/genders/auto/Libgenders/autosplit.ix
