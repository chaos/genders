##*****************************************************************************
## $Id: ac_meta.m4,v 1.2 2006-03-28 02:27:22 chu11 Exp $
##*****************************************************************************
#  AUTHOR:
#    Chris Dunlap <cdunlap@llnl.gov>
#
#  SYNOPSIS:
#    AC_META
#
#  DESCRIPTION:
#    Set PROJECT and VERSION from the META file.
##*****************************************************************************

AC_DEFUN([AC_META],
[
  PROJECT="`perl -ne 'print,exit if s/^\s*NAME:\s*(\S*).*/\1/i' $srcdir/META`"
  AC_DEFINE_UNQUOTED([PROJECT], ["$PROJECT"], [Define the project name.])
  AC_SUBST([PROJECT])

  VERSION="`perl -ne 'print,exit if s/^\s*VERSION:\s*(\S*).*/\1/i' $srcdir/META`"
  AC_DEFINE_UNQUOTED([VERSION], ["$VERSION"], [Define the project version.])
  AC_SUBST([VERSION])

  RELEASE="`perl -ne 'print,exit if s/^\s*RELEASE:\s*(\S*).*/\1/i' $srcdir/META`"
  AC_DEFINE_UNQUOTED([RELEASE], ["$RELEASE"], [Define the project's release.])
  AC_SUBST([RELEASE])
])
