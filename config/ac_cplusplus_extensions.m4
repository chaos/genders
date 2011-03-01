##*****************************************************************************
## $Id: ac_cplusplus_extensions.m4,v 1.1 2009-02-19 00:50:17 chu11 Exp $
##*****************************************************************************

AC_DEFUN([AC_CPLUSPLUS_EXTENSIONS],
[
  AC_MSG_CHECKING(for --with-cplusplus-extensions)
  AC_ARG_WITH(cplusplus-extensions,
     AC_HELP_STRING([--with-cplusplus-extensions=], 
                    [enable or disable c++ extensions build]),
     [ case "$withval" in
     yes)
         ac_with_cplusplus_extensions=yes
         ;;
     no)
         ac_with_cplusplus_extensions=no
         ;;
     *)
         ac_with_cplusplus_extensions=yes
         ;;
     esac ]
  )
  AC_MSG_RESULT(${ac_with_cplusplus_extensions=yes})
])
