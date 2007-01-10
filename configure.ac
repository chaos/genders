##*****************************************************************************
## $Id: configure.ac,v 1.44 2007-01-10 18:58:37 chu11 Exp $
##*****************************************************************************
## Process this file with autoconf to produce a configure script.
##*****************************************************************************

##
# Prologue.
##
AC_INIT([genders], [1.6])
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
LIBGENDERS_CURRENT=1
LIBGENDERS_REVISION=0
LIBGENDERS_AGE=1
LIBGENDERS_VERSION_INFO=$LIBGENDERS_CURRENT:$LIBGENDERS_REVISION:$LIBGENDERS_AGE
AC_SUBST([LIBGENDERS_VERSION_INFO])

# Libgenders and Genders versioning
PERLGENDERS_MAJOR=0
PERLGENDERS_MINOR=02
PERLGENDERS_VERSION=$PERLGENDERS_MAJOR.$PERLGENDERS_MINOR
AC_SUBST([PERLGENDERS_VERSION])

##
# Checks for programs.
##
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AM_PROG_LEX
AC_PROG_YACC
AM_CONDITIONAL(WITH_GNU_LD, test "$with_gnu_ld" = "yes")
AC_PATH_PROG([PERL], [perl])
AC_PATH_PROG([POD2MAN], [pod2man], [/usr/bin/pod2man], [$PATH:/usr/perl5/bin/])
AC_DEBUG

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
# Check for perl destdir
#
AC_PERL_DESTDIR

#
# Determine Perl section-3 man page extension
#
PERLMAN3EXT=`$PERL -MConfig -e 'print $Config{man3ext}'`
AC_SUBST([PERLMAN3EXT])

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
  src/Libgenders/Makefile \
  src/Genders/Makefile \
  src/testsuite/Makefile \
  src/testsuite/libgenders/Makefile \
  src/testsuite/libgenders/testdatabases/Makefile \
  compat/Makefile \
  man/Makefile \
  man/compat/Makefile \
  src/Libgenders/Libgenders.pm \
  src/Genders/Genders.pm \
)
AC_OUTPUT

