##*****************************************************************************
## $Id: ac_genders_file.m4,v 1.2 2005-07-18 21:23:23 achu Exp $
##*****************************************************************************

AC_DEFUN([AC_GENDERS_FILE],
[
  GENDERS_DEFAULT_FILE="\"/etc/genders\""
  AC_MSG_CHECKING([for genders default file path])
  AC_ARG_WITH([genders-file],
    AC_HELP_STRING([--with-genders-file=PATH], 
                   [define default genders file]),
    [ case "$withval" in
        no)  ;;
        yes) ;;
        *)   GENDERS_DEFAULT_FILE="\"$withval\"" 
      esac
    ]
  )
  AC_MSG_RESULT($GENDERS_DEFAULT_FILE)

  AC_SUBST(GENDERS_DEFAULT_FILE)
])
