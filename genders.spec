Name:    genders 
Version: 1.26
Release: 1
Summary: Static cluster configuration database
URL: https://github.com/chaos/genders
Group: System Environment/Base
License: GPL
Source: %{name}-%{version}.tar.gz
Requires: perl
BuildRequires: bison flex
BuildRequires: perl(ExtUtils::MakeMaker)
BuildRequires: python
BuildRequires: python-devel
BuildRequires: libtool
BuildRoot: %{_tmppath}/%{name}-%{version}

%description
Genders is a static cluster configuration database used for cluster
configuration management.  It is used by a variety of tools and
scripts for management of large clusters.  The genders database is
typically replicated on every node of the cluster. It describes the
layout and configuration of the cluster so that tools and scripts can
sense the variations of cluster nodes. By abstracting this information
into a plain text file, it becomes possible to change the
configuration of a cluster by modifying only one file.

%package compat
Summary: Compatibility library 
Group: System Environment/Base
%description compat
genders API that is compatible with earlier releases of genders

%{!?_with_perl_extensions: %{!?_without_perl_extensions: %define _with_perl_extensions --with-perl-extensions}}
%{!?_with_python_extensions: %{!?_without_python_extensions: %define _with_python_extensions --with-python-extensions}}
%{!?_with_cplusplus_extensions: %{!?_without_cplusplus_extensions: %define _with_cplusplus_extensions --with-cplusplus-extensions}}
%{!?_with_java_extensions: %{!?_without_java_extensions: %define _without_java_extensions --without-java-extensions}}
%{!?_with_lua_extensions: %{!?_without_lua_extensions: %define _without_lua_extensions --without-lua-extensions}}

# choose vendor arch by default
%{!?_with_perl_site_arch: %{!?_with_perl_vendor_arch: %define _with_perl_vendor_arch --with-perl-vendor-arch}}

%prep
%setup  -q -n %{name}-%{version}

%build
%configure --program-prefix=%{?_program_prefix:%{_program_prefix}} \
    --with-extension-destdir="$RPM_BUILD_ROOT" \
    %{?_with_perl_extensions} \
    %{?_without_perl_extensions} \
    %{?_with_perl_site_arch} \
    %{?_without_perl_site_arch} \
    %{?_with_perl_vendor_arch} \
    %{?_without_perl_vendor_arch} \
    %{?_with_python_extensions} \
    %{?_without_python_extensions} \
    %{?_with_cplusplus_extensions} \
    %{?_without_cplusplus_extensions} \
    %{?_with_java_extensions} \
    %{?_without_java_extensions}\
    %{?_with_lua_extensions} \
    %{?_without_lua_extension}
make 

%install
rm -rf $RPM_BUILD_ROOT
DESTDIR="$RPM_BUILD_ROOT" make install 

%files
%defattr(-,root,root)
%doc README NEWS ChangeLog DISCLAIMER DISCLAIMER.UC COPYING TUTORIAL genders.sample
%if %{?_with_java_extensions:1}%{!?_with_java_extensions:0}
%dir %{_datadir}/doc/%{name}-%{version}-javadoc/
%doc %{_datadir}/doc/%{name}-%{version}-javadoc/*
%endif
# It doesn't matter if the user chooses a 32bit or 64bit target.  The
# packaging must work off whatever Perl is installed.  
%if %{?_with_perl_site_arch:1}%{!?_with_perl_site_arch:0}
%define _perldir %(perl -e 'use Config; $T=$Config{installsitearch}; $P=$Config{siteprefix}; $T=~/$P\\/(.*)/; print "%{_prefix}/$1\\n"')
%endif
%if %{?_with_perl_vendor_arch:1}%{!?_with_perl_vendor_arch:0}
%define _perldir %(perl -e 'use Config; $T=$Config{installvendorarch}; $P=$Config{vendorprefix}; $T=~/$P\\/(.*)/; print "%{_prefix}/$1\\n"')
%endif
%{_mandir}/man1/*
%{_mandir}/man3/genders*
%{_mandir}/man3/libgenders* 
%{_includedir}/*
%{_bindir}/*
%{_libdir}/libgenders.*
%if %{?_with_perl_extensions:1}%{!?_with_perl_extensions:0}
%{_mandir}/man3/Libgenders*
%{_mandir}/man3/Genders*
%{_perldir}/*
%endif
%if %{?_with_python_extensions:1}%{!?_with_python_extensions:0}
%{_exec_prefix}/lib*/python*
%endif
%if %{?_with_cplusplus_extensions:1}%{!?_with_cplusplus_extensions:0}
%{_libdir}/libgendersplusplus.*
%endif
%if %{?_with_java_extensions:1}%{!?_with_java_extensions:0}
%{_javadir}/*
%{_libdir}/libGendersjni.*
%endif

%files compat
%defattr(-,root,root)
%{_mandir}/man3/gendlib*
%{_prefix}/lib/genders/*


