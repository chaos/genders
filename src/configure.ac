##*****************************************************************************
## $Id: configure.ac,v 1.1 2003-02-10 22:15:25 achu Exp $
##*****************************************************************************
## Process this file with autoconf to produce a configure script.
##*****************************************************************************

##
# Prologue.
##
AC_INIT([genders], [0.0], [chu11@llnl.gov])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([config])
AC_CANONICAL_SYSTEM

##
# Automake support.
##
AM_INIT_AUTOMAKE([check-news dist-bzip2 gnu])
AM_CONFIG_HEADER([config/config.h])
AM_MAINTAINER_MODE

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
  strchr \
)

##
# Epilogue.
##
AC_CONFIG_FILES( \
  Makefile \
  libgenders/Makefile \
)
AC_OUTPUT

