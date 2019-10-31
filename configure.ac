##*****************************************************************************
## $Id: configure.ac,v 1.56 2010-03-24 21:32:37 chu11 Exp $
##*****************************************************************************
## Process this file with autoconf to produce a configure script.
##*****************************************************************************

##
# Prologue.
##
AC_INIT([genders],[1.27])
AC_META
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([README])
AC_CANONICAL_TARGET

##
# Automake support.
##
AM_INIT_AUTOMAKE(tar-ustar)
AM_CONFIG_HEADER([config/config.h])
AM_MAINTAINER_MODE

# Update this information before a public release only!

# libgenders versioning
#
# If the library source code has changed at all since the last update,
# then increment c:r: c:r+1:).                                                                                     
#
# If any interfaces have been added, removed, or changed since the
# last update, increment current, and set revision to 0.
#
# If any interfaces have been added since the last public release,
# then increment age.
#
# If any interfaces have been removed or changed since the last public                                                
# release, then set age to 0.
#                                               
LIBGENDERS_CURRENT=3
LIBGENDERS_REVISION=3
LIBGENDERS_AGE=3
LIBGENDERS_VERSION_INFO=$LIBGENDERS_CURRENT:$LIBGENDERS_REVISION:$LIBGENDERS_AGE
AC_SUBST([LIBGENDERS_VERSION_INFO])

# C++ library
LIBGENDERSPLUSPLUS_CURRENT=2
LIBGENDERSPLUSPLUS_REVISION=0
LIBGENDERSPLUSPLUS_AGE=0
LIBGENDERSPLUSPLUS_VERSION_INFO=$LIBGENDERSPLUSPLUS_CURRENT:$LIBGENDERSPLUSPLUS_REVISION:$LIBGENDERSPLUSPLUS_AGE
AC_SUBST([LIBGENDERSPLUSPLUS_VERSION_INFO])

# Java JNI C library
LIBGENDERSJNI_CURRENT=0
LIBGENDERSJNI_REVISION=0
LIBGENDERSJNI_AGE=0
LIBGENDERSJNI_VERSION_INFO=$LIBGENDERSJNI_CURRENT:$LIBGENDERSJNI_REVISION:$LIBGENDERSJNI_AGE
AC_SUBST([LIBGENDERSJNI_VERSION_INFO])

# Perl Libgenders and Genders versioning
PERLGENDERS_MAJOR=0
PERLGENDERS_MINOR=03
PERLGENDERS_VERSION=$PERLGENDERS_MAJOR.$PERLGENDERS_MINOR
AC_SUBST([PERLGENDERS_VERSION])

# Python genders versioning
PYTHONGENDERS_MAJOR=1
PYTHONGENDERS_MINOR=2
PYTHONGENDERS_VERSION=$PYTHONGENDERS_MAJOR.$PYTHONGENDERS_MINOR
AC_SUBST([PYTHONGENDERS_VERSION])

##
# Checks for programs.
##
AC_PROG_CC
AC_PROG_CXX
LT_INIT
AC_PROG_MAKE_SET
AM_PROG_LEX
AC_PROG_YACC
AM_CONDITIONAL(WITH_GNU_LD, test "$with_gnu_ld" = "yes")
AC_PATH_PROG([PERL], [perl])
AC_PATH_PROG([PYTHON], [python])
AC_PATH_PROG([POD2MAN], [pod2man], [/usr/bin/pod2man], [$PATH:/usr/perl5/bin/])
AC_PATH_PROG([JAVAC], [javac])
AC_PATH_PROG([JAVAH], [javah])
AC_PATH_PROG([JAVA], [java])
AC_PATH_PROG([JAR], [jar])
AC_PATH_PROG([JAVADOC], [javadoc])
AC_DEBUG

##
## achu: Solaris 10 (and maybe some other earlier Solarises) bison has
## a bug which segfaults.  So we use the below to force yacc usage
## when we need to .
##
AC_MSG_CHECKING(for --with-force-yacc )
AC_ARG_WITH(force-yacc,
   AS_HELP_STRING([--with-force-yacc],[force yacc to be used]),
   [ case "$withval" in
   yes)
       ac_force_yacc=yes
       ;;
   no)
       ac_force_yacc=no
       ;;
   *)
       ac_force_yacc=no
       ;;
   esac ]
) 
AC_MSG_RESULT(${ac_force_yacc=no})
if test "$ac_force_yacc" = yes; then
  YACC=yacc
  AC_SUBST(FORCE_YACC)
fi

##
##
##
AC_MSG_CHECKING(for --with-non-shortened-hostnames)
AC_ARG_WITH(non-shortened-hostnames,
   AS_HELP_STRING([--with-non-shortened-hostnames],[allow non-shortened hostnames with periods]),
   [ case "$withval" in
   yes)
       ac_non_shortened_hostnames=yes
       ;;
   no)
       ac_non_shortened_hostnames=no
       ;;
   *)
       ac_non_shortened_hostnames=no
       ;;
   esac ]
) 
AC_MSG_RESULT(${ac_non_shortened_hostnames=no})

if test x"${ac_non_shortened_hostnames}" = xyes; then
   AC_DEFINE([WITH_NON_SHORTENED_HOSTNAMES], [1], [Define if you want to support non shortened hostnames])
fi

##
# Checks for header files.
##
AC_HEADER_STDC
AC_CHECK_HEADERS( \
  fcntl.h \
  unistd.h \
  getopt.h \
  paths.h \
)

#
# Check for default paths
#
AC_GENDERS_FILE

#
# Check for extension stuff
#
AC_EXTENSION_DESTDIR

#
# Check for perl stuff
#
AC_PERL_EXTENSIONS
AM_CONDITIONAL(WITH_PERL_EXTENSIONS, [test "$ac_with_perl_extensions" = "yes"])

#
# Determine Perl section-3 man page extension
#
PERLMAN3EXT=`$PERL -MConfig -e 'print $Config{man3ext}'`
AC_SUBST([PERLMAN3EXT])

#
# Determine Perl Install Location
#
AC_PERL_SITE_ARCH
AC_PERL_VENDOR_ARCH
if test "$ac_with_perl_site_arch" = yes && \
   test "$ac_with_perl_vendor_arch" = yes ; then
   AC_MSG_ERROR([cannot specify both perl site arch and vendor arch])
elif test "$ac_with_perl_site_arch" = yes; then
   PERL_ARCH_INSTALL=site
elif test "$ac_with_perl_vendor_arch" = yes; then
   PERL_ARCH_INSTALL=vendor
else
   PERL_ARCH_INSTALL=vendor
fi
AC_SUBST(PERL_ARCH_INSTALL)

#
# Check for python stuff
#
AC_PYTHON_EXTENSIONS
AM_CONDITIONAL(WITH_PYTHON_EXTENSIONS, [test "$ac_with_python_extensions" = "yes"])

#
# Check for c++ stuff
#
AC_CPLUSPLUS_EXTENSIONS
AM_CONDITIONAL(WITH_CPLUSPLUS_EXTENSIONS, [test "$ac_with_cplusplus_extensions" = "yes"])

#
# Check for java stuff
#
AC_JAVA_EXTENSIONS
AM_CONDITIONAL(WITH_JAVA_EXTENSIONS, [test "$ac_with_java_extensions" = "yes"])

##
# Checks for typedefs, structures, and compiler characteristics.
##
AC_C_BIGENDIAN
AC_C_CONST
AC_TYPE_UID_T

##
# Checks for library functions.
##
AC_CHECK_FUNCS( \
  strcmp \
  strlen \
  strcpy \
  strchr \
  strsep \
  strspn \
  strtok_r \
  getopt_long \
)

##
# Epilogue.
##
AC_CONFIG_FILES( \
  genders.spec \
  Makefile \
  src/Makefile \
  src/libcommon/Makefile \
  src/libgenders/Makefile \
  src/nodeattr/Makefile \
  src/libgenders/genders.h \
  src/extensions/Makefile \
  src/extensions/cplusplus/Makefile \
  src/extensions/perl/Makefile \
  src/extensions/perl/Libgenders/Makefile \
  src/extensions/perl/Libgenders/Libgenders.pm \
  src/extensions/perl/Genders/Makefile \
  src/extensions/perl/Genders/Genders.pm \
  src/extensions/python/Makefile \
  src/extensions/python/genderssetup.py \
  src/extensions/java/Makefile \
  src/testsuite/Makefile \
  src/testsuite/libgenders/Makefile \
  src/testsuite/libgenders/testdatabases/Makefile \
  compat/Makefile \
  contrib/Makefile \
  contrib/cfengine/Makefile \
  contrib/cfengine/cfengine-2.2.10/Makefile \
  contrib/cfengine/cfengine-3.3.0/Makefile \
  contrib/php/Makefile \
  man/Makefile \
  man/compat/Makefile \
)
AC_OUTPUT

