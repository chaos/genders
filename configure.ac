##*****************************************************************************
## $Id: configure.ac,v 1.31 2005-01-10 16:45:35 achu Exp $
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
LIBGENDERS_CURRENT=0
LIBGENDERS_REVISION=0
LIBGENDERS_AGE=0
LIBGENDERS_VERSION_INFO=$LIBGENDERS_CURRENT:$LIBGENDERS_REVISION:$LIBGENDERS_AGE
AC_SUBST([LIBGENDERS_VERSION_INFO])

# Libgenders and Genders versioning
PERLGENDERS_MAJOR=0
PERLGENDERS_MINOR=01
PERLGENDERS_VERSION=$PERLGENDERS_MAJOR.$PERLGENDERS_MINOR
AC_SUBST([PERLGENDERS_VERSION])

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

# Perl Install Destdir
# - Used almost exclusively so perl install works with rpm builds
AC_MSG_CHECKING(for --with-perl-install-destdir )
AC_ARG_WITH(perl-install-destdir,
    AC_HELP_STRING([--with-perl-install-destdir=path], [define perl modules install prefix]),
    [ case "$withval" in
    yes)
        withperllinstalldestdir=yes
        ;;
    no)
        ;;
    *)
        withperllinstalldestdir=yes
        PERL_INSTALL_DESTDIR="$withval"
        ;;
    esac ]
)
AC_MSG_RESULT(${withperllinstalldestdir=no})
if test -z "$PERL_INSTALL_DESTDIR"; then
   PERL_INSTALL_PREFIX='${prefix}'
else
   PERL_INSTALL_PREFIX='${PERL_INSTALL_DESTDIR}${prefix}'
fi
AC_SUBST(PERL_INSTALL_DESTDIR)
AC_SUBST(PERL_INSTALL_PREFIX)

##
# Checks for programs.
##
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AM_PROG_LEX
AC_PROG_YACC
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
  src/testsuite/Makefile \
  src/testsuite/libgenders/Makefile \
  compat/Makefile \
  man/Makefile \
  man/compat/Makefile \
  src/Libgenders/Libgenders.pm \
  src/Genders/Genders.pm \
)
AC_OUTPUT

