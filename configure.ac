##*****************************************************************************
## $Id: configure.ac,v 1.56 2010-03-24 21:32:37 chu11 Exp $
##*****************************************************************************
## Process this file with autoconf to produce a configure script.
##*****************************************************************************

##
# Prologue.
##
AC_INIT([genders], [1.17])
AC_META
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([README])
AC_CANONICAL_SYSTEM

##
# Automake support.
##
AM_INIT_AUTOMAKE(tar-ustar)
AM_CONFIG_HEADER([config/config.h])
AM_MAINTAINER_MODE

# Update this information before a public release only!

# libgenders versioning
#
# If library source code has changed at all increment rev
# (current:rev:age) = (current:rev+1:age)
#
# If any interfaces have been added,removed, or changed increment current
#  and set rev = 0;
#
# If any interfaces have been added increment age
#
# If any interfaces have been removed, set age = 0
#
LIBGENDERS_CURRENT=3
LIBGENDERS_REVISION=0
LIBGENDERS_AGE=3
LIBGENDERS_VERSION_INFO=$LIBGENDERS_CURRENT:$LIBGENDERS_REVISION:$LIBGENDERS_AGE
AC_SUBST([LIBGENDERS_VERSION_INFO])

# C++ library
LIBGENDERSPLUSPLUS_CURRENT=1
LIBGENDERSPLUSPLUS_REVISION=0
LIBGENDERSPLUSPLUS_AGE=0
LIBGENDERSPLUSPLUS_VERSION_INFO=$LIBGENDERSPLUSPLUS_CURRENT:$LIBGENDERSPLUSPLUS_REVISION:$LIBGENDERSPLUSPLUS_AGE
AC_SUBST([LIBGENDERSPLUSPLUS_VERSION_INFO])

# Perl Libgenders and Genders versioning
PERLGENDERS_MAJOR=0
PERLGENDERS_MINOR=03
PERLGENDERS_VERSION=$PERLGENDERS_MAJOR.$PERLGENDERS_MINOR
AC_SUBST([PERLGENDERS_VERSION])

# Python Libgenders and Genders versioning
PYTHONGENDERS_MAJOR=1
PYTHONGENDERS_MINOR=0
PYTHONGENDERS_VERSION=$PYTHONGENDERS_MAJOR.$PYTHONGENDERS_MINOR
AC_SUBST([PYTHONGENDERS_VERSION])

##
# Checks for programs.
##
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AM_PROG_LEX
AC_PROG_YACC
AM_CONDITIONAL(WITH_GNU_LD, test "$with_gnu_ld" = "yes")
AC_PATH_PROG([PERL], [perl])
AC_PATH_PROG([PYTHON], [python])
AC_PATH_PROG([POD2MAN], [pod2man], [/usr/bin/pod2man], [$PATH:/usr/perl5/bin/])
AC_DEBUG

##
## achu: Solaris 10 (and maybe some other earlier Solarises) bison has
## a bug which segfaults.  So we use the below to force yacc usage
## when we need to .
##
AC_MSG_CHECKING(for --with-force-yacc )
AC_ARG_WITH(force-yacc,
   AC_HELP_STRING([--with-force-yacc],
                  [force yacc to be used]),
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
   PERL_ARCH_INSTALL=site
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
  src/testsuite/Makefile \
  src/testsuite/libgenders/Makefile \
  src/testsuite/libgenders/testdatabases/Makefile \
  compat/Makefile \
  contrib/Makefile \
  contrib/php/Makefile \
  man/Makefile \
  man/compat/Makefile \
)
AC_OUTPUT

