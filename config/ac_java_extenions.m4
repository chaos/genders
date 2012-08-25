##*****************************************************************************
## $Id: ac_java_extensions.m4,v 1.1 2009-02-19 00:50:17 chu11 Exp $
##*****************************************************************************

AC_DEFUN([AC_JAVA_EXTENSIONS],
[
  AC_MSG_CHECKING(for --with-java-extensions)
  AC_ARG_WITH(java-extensions,
     AC_HELP_STRING([--with-java-extensions=], 
                    [enable or disable java extensions build]),
     [ case "$withval" in
     yes)
         ac_with_java_extensions=yes
         ;;
     no)
         ac_with_java_extensions=no
         ;;
     *)
         ac_with_java_extensions=yes
         ;;
     esac ]
  )
  AC_MSG_RESULT(${ac_with_java_extensions=yes})
])
