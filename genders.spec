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
./configure --program-prefix=%{?_program_prefix:%{_program_prefix}}
make 

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/man/man1
mkdir -p $RPM_BUILD_ROOT/usr/man/man3
install -d $RPM_BUILD_ROOT/usr/lib/genders
install compat/gendlib.pl $RPM_BUILD_ROOT/usr/lib/genders
install compat/hostlist.pl $RPM_BUILD_ROOT/usr/lib/genders
install -s inst $RPM_BUILD_ROOT/usr/bin
install dist/dist2 $RPM_BUILD_ROOT/usr/bin
install dist/dist_all $RPM_BUILD_ROOT/usr/bin
install dist/dist_local $RPM_BUILD_ROOT/usr/bin
install dist/dist_cmp $RPM_BUILD_ROOT/usr/bin
install dist/dist_bootstrap $RPM_BUILD_ROOT/usr/bin
cd man/compat/; gzip *; cd ../..
install man/compat/gendlib.3.gz $RPM_BUILD_ROOT/usr/man/man3
cd man/dist/; gzip *; cd ../..
install man/dist/dist2.1.gz $RPM_BUILD_ROOT/usr/man/man1
install man/dist/dist_all.1.gz $RPM_BUILD_ROOT/usr/man/man1
install man/dist/dist_cmp.1.gz $RPM_BUILD_ROOT/usr/man/man1
install man/dist/dist_local.1.gz $RPM_BUILD_ROOT/usr/man/man1
install man/dist/inst.1.gz $RPM_BUILD_ROOT/usr/man/man1
install man/dist/nodeattr.1.gz $RPM_BUILD_ROOT/usr/man/man1
cd man/; gzip *; cd ..
install man/genders_attrlist_clear.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_attrlist_create.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_attrlist_destroy.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_errormsg.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_errnum.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getattr.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getattr_all.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getmaxattrlen.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getmaxattrs.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getmaxnodelen.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getmaxvallen.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getnodename.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getnodes.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getnumattrs.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_getnumnodes.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_handle_create.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_handle_destroy.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_handle_dump.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_nodelist_clear.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_nodelist_create.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_nodelist_destroy.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_load_data.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_perror.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_strerror.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_testattr.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_testattrval.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_testnode.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_vallist_clear.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_vallist_create.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_vallist_destroy.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_list.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_stats.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_errors.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/genders_parse.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/libgenders.3.gz $RPM_BUILD_ROOT/usr/man/man3
install man/Libgenders.3.gz $RPM_BUILD_ROOT/usr/man/man3
cp src/libgenders/genders.h src/Libgenders	
cp src/libgenders/genders.c src/Libgenders	
cd src/Libgenders; perl Makefile.PL "prefix=$RPM_BUILD_ROOT"; make; make install; cd ../../..
DESTDIR="$RPM_BUILD_ROOT" make install 

%files
%defattr(-,root,root)
%doc README ChangeLog DISCLAIMER COPYING
/usr/man/man3/*
/usr/lib/libgenders.*
/usr/lib/perl5/*
/usr/include/genders.h

%files compat
%defattr(-,root,root)
%doc TUTORIAL genders.sample clusters.sample Distfile.sample
/usr/lib/genders/gendlib.pl
/usr/lib/genders/hostlist.pl

%files dist
%defattr(-,root,root)
/usr/man/man1/*
/usr/bin/*
