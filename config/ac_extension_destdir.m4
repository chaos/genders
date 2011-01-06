##*****************************************************************************
## $Id: ac_destdir_destdir.m4,v 1.1 2006-07-17 22:06:43 chu11 Exp $
##*****************************************************************************

# Used almost exclusively so extensions like perl and python install works
# with rpm builds
AC_DEFUN([AC_EXTENSION_DESTDIR],
[
  AC_MSG_CHECKING(for --with-extension-destdir )
  AC_ARG_WITH(extension-destdir,
     AC_HELP_STRING([--with-extension-destdir=path], 
                    [define extension modules install prefix]),
     [ case "$withval" in
     yes)
         withextensionldestdir=yes
         ;;
     no)
         ;;
     *)
         withextensionldestdir=yes
         EXTENSION_DESTDIR="$withval"
         ;;
     esac ]
  )
  AC_MSG_RESULT(${withextensionldestdir=no})
  AC_SUBST(EXTENSION_DESTDIR)
])
