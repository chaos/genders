##*****************************************************************************
## $Id: configure.ac,v 1.20 2003-09-11 15:18:51 achu Exp $
##*****************************************************************************
## Process this file with autoconf to produce a configure script.
##*****************************************************************************

##
# Prologue.
##
AC_INIT([genders], [1.1], [chu11@llnl.gov])
AC_CONFIG_AUX_DIR([config])
AC_CANONICAL_SYSTEM

PROJECT="`perl -ne 'print,exit if s/^\s*NAME:\s*(\S*).*/\1/i' $srcdir/META`"
AC_DEFINE_UNQUOTED([PROJECT], ["$PROJECT"], [Define the package name.])
AC_SUBST([PROJECT])

VERSION="`perl -ne 'print,exit if s/^\s*VERSION:\s*(\S*).*/\1/i' $srcdir/META`"
AC_DEFINE_UNQUOTED([VERSION], ["$VERSION"], [Define the package version.])
AC_SUBST([VERSION])

RELEASE="`perl -ne 'print,exit if s/^\s*RELEASE:\s*(\S*).*/\1/i' $srcdir/META`"
AC_DEFINE_UNQUOTED([RELEASE], ["$RELEASE"], [Define the project's release.])
AC_SUBST([RELEASE])

##
# Automake support.
##
AM_INIT_AUTOMAKE($PROJECT, $VERSION)
AM_CONFIG_HEADER([config/config.h])
AM_MAINTAINER_MODE

# option for default genders file
AC_MSG_CHECKING(for --with-genders-file )
AC_ARG_WITH(genders-file,
    AC_HELP_STRING([--with-genders-file=file], [define default genders file]),
    [ case "$withval" in
    yes)
        withgendersfile=yes
        ;;
    no)
        ;;
    *)
        withgendersfile=yes
        GENDERS_DEFAULT_FILE="\"$withval\""
        ;;
    esac ]
)
AC_MSG_RESULT(${withgendersfile=no})
if test -z "$GENDERS_DEFAULT_FILE"; then
   GENDERS_DEFAULT_FILE="\"/etc/genders\""
fi
AC_SUBST(GENDERS_DEFAULT_FILE)

# Libgenders install location
AC_MSG_CHECKING(for --with-xs-prefix )
AC_ARG_WITH(xs-prefix,
    AC_HELP_STRING([--with-xs-prefix=path], [define perl xs prefix dir]),
    [ case "$withval" in
    yes)
        withxsprefix=yes
        ;;
    no)
        ;;
    *)
        withxsprefix=yes
        XS_PREFIX="$withval"
        ;;
    esac ]
)
AC_MSG_RESULT(${withxsprefix=no})
if test -z "$XS_PREFIX"; then
   XS_PREFIX='${prefix}'
fi
AC_SUBST(XS_PREFIX)

# Genders.pm install location
AC_MSG_CHECKING(for --with-genders-pm-prefix )
AC_ARG_WITH(genders-pm-prefix,
    AC_HELP_STRING([--with-genders-pm-prefix=path], [define genders.pm prefix dir]),
    [ case "$withval" in
    yes)
        withgenderspm=yes
        ;;
    no)
        ;;
    *)
        withgenderspm=yes
        GENDERS_PM_PREFIX="$withval"
        ;;
    esac ]
)
AC_MSG_RESULT(${withgenderspm=no})
if test -z "$GENDERS_PM_PREFIX"; then
   GENDERS_PM_PREFIX='${prefix}'
fi
AC_SUBST(GENDERS_PM_PREFIX)

##
# Checks for programs.
##
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AC_PATH_PROG(PERL, perl)

##
# Checks for header files.
##
AC_HEADER_STDC
AC_CHECK_HEADERS( \
  errno.h \
  fcntl.h \
  stdio.h \
  stdlib.h \
  string.h \
  unistd.h \
  getopt.h \
  stat.h \
  types.h \
)

##
# Checks for typedefs, structures, and compiler characteristics.
##
AC_C_BIGENDIAN
AC_C_CONST
AC_TYPE_UID_T

##
# Checks for library functions.
##
AC_FUNC_MALLOC
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
  src/libgenders/Makefile \
  src/nodeattr/Makefile \
  src/libgenders/genders.h \
  src/Libgenders/Makefile \
  src/Genders/Makefile \
  compat/Makefile \
  man/Makefile \
  man/compat/Makefile \
)
AC_OUTPUT

