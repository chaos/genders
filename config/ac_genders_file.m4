##*****************************************************************************
## $Id: ac_genders_file.m4,v 1.1 2005-04-19 23:47:27 achu Exp $
##*****************************************************************************

AC_DEFUN([AC_GENDERS_FILE],
[
  AC_MSG_CHECKING([for genders default file path])
  AC_ARG_WITH([genders-file],
    AC_HELP_STRING([--with-genders-file=PATH], 
                   [define default genders file]),
    [ case "$withval" in
        no)  GENDERS_DEFAULT_FILE="\"/etc/genders\"" ;;
        yes) GENDERS_DEFAULT_FILE="\"/etc/genders\"" ;;
        *)   GENDERS_DEFAULT_FILE="\"$withval\"" 
      esac
    ]
  )
  AC_MSG_RESULT([${GENDERS_DEFAULT_FILE="\"/etc/genders\""}])

  AC_SUBST(GENDERS_DEFAULT_FILE)
])
