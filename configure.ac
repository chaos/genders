##*****************************************************************************
## $Id: configure.ac,v 1.27 2004-04-27 17:49:00 achu Exp $
##*****************************************************************************
## Process this file with autoconf to produce a configure script.
##*****************************************************************************

##
# Prologue.
##
AC_INIT([genders])
AC_META
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([README])
AC_CANONICAL_SYSTEM

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

# Perl Install Prefix
# - Used almost exclusively so perl install works with rpm builds
AC_MSG_CHECKING(for --with-perl-install-prefix )
AC_ARG_WITH(perl-install-prefix,
    AC_HELP_STRING([--with-perl-install-prefix=path], [define perl modules install prefix]),
    [ case "$withval" in
    yes)
        withperllinstallprefix=yes
        ;;
    no)
        ;;
    *)
        withperllinstallprefix=yes
        PERL_INSTALL_PREFIX="$withval"
        ;;
    esac ]
)
AC_MSG_RESULT(${withperllinstallprefix=no})
if test -z "$PERL_INSTALL_PREFIX"; then
   PERL_INSTALL_PREFIX='${prefix}'
fi
AC_SUBST(PERL_INSTALL_PREFIX)

##
# Checks for programs.
##
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AM_CONDITIONAL(WITH_GNU_LD, test "$with_gnu_ld" = "yes")
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

# Do this last, you could add compile options which can make
# function checks fail
AC_DEBUG

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

