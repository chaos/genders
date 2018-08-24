##*****************************************************************************
## $Id: ac_python_extensions.m4,v 1.1 2009-02-19 00:50:17 chu11 Exp $
##*****************************************************************************

AC_DEFUN([AC_PYTHON_EXTENSIONS],
[
  AC_MSG_CHECKING(for --with-python-extensions)
  AC_ARG_WITH(python-extensions,
     AC_HELP_STRING([--with-python-extensions=], 
                    [enable or disable python extensions build]),
     [ case "$withval" in
     yes)
         ac_with_python_extensions=yes
         ;;
     no)
         ac_with_python_extensions=no
         ;;
     *)
         ac_with_python_extensions=yes
         ;;
     esac ]
  )
  AC_MSG_RESULT(${ac_with_python_extensions=yes})
])
