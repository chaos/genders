##*****************************************************************************
## $Id: configure.ac,v 1.6 2003-05-12 23:52:01 achu Exp $
##*****************************************************************************
## Process this file with autoconf to produce a configure script.
##*****************************************************************************

##
# Prologue.
##
AC_INIT([genders], [1.0], [chu11@llnl.gov])
AC_CONFIG_AUX_DIR([config])
AC_CANONICAL_SYSTEM

##
# Automake support.
##
AM_INIT_AUTOMAKE([check-news dist-bzip2 gnu])
AM_CONFIG_HEADER([config/config.h])
AM_MAINTAINER_MODE

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

##
# Checks for programs.
##
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET

##
# Checks for header files.
##
AC_HEADER_STDC
AC_CHECK_HEADERS( \
  errno.h \
  stdlib.h \
  string.h \
  unistd.h \
  unistd.h \
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
)

##
# Epilogue.
##
AC_CONFIG_FILES( \
  Makefile \
  src/Makefile \
  src/libgenders/Makefile \
  src/nodeattr/Makefile \
  src/libgenders/genders.h \
  dist/Makefile \
)
AC_OUTPUT

