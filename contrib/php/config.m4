dnl $Id: config.m4,v 1.1 2007-10-03 20:20:14 chu11 Exp $
dnl config.m4 for extension genders

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

PHP_ARG_WITH(genders, for genders support,
[  --with-genders[=DIR]      Include genders support])

if test "$PHP_GENDERS" != "no"; then

  if test "$PHP_GENDERS" != "yes"; then
    AC_MSG_CHECKING(if genders specified path is valid)
    if test -r $PHP_GENDERS/include/genders.h; then
      GENDERS_DIR=$PHP_GENDERS
      AC_MSG_RESULT([yes])
    else
    AC_MSG_RESULT([no])
      AC_MSG_ERROR([The specified genders path is Invalid or the installation is incomplete
      Please specify another path or reinstall the genders distribution])
    fi
  else
    SEARCH_PATH="/usr/local/genders /usr/local /usr /opt"
    SEARCH_FOR="/include/genders.h"
    if test -r $PHP_GENDERS/$SEARCH_FOR; then # path given as parameter
      GENDERS_DIR=$PHP_GENDERS
    else # search default path list
      AC_MSG_CHECKING([for genders files in default path])
      for i in $SEARCH_PATH ; do
        if test -r $i/$SEARCH_FOR; then
          GENDERS_DIR=$i
          AC_MSG_RESULT(found in $i)
        fi
      done
    fi
  fi
 
  if test -z "$GENDERS_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the genders distribution])
  fi

  PHP_ADD_INCLUDE($GENDERS_DIR/include)

  LIBNAME=genders
  LIBSYMBOL=genders_handle_create

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $GENDERS_DIR/lib, GENDERS_SHARED_LIBADD)
    AC_DEFINE(HAVE_GENDERSLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong genders lib version or lib not found])
  ],[
    -L$GENDERS_DIR/lib -lm -ldl
  ])
 
  PHP_SUBST(GENDERS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(genders, genders.c, $ext_shared)
fi
