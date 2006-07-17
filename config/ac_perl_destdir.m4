##*****************************************************************************
## $Id: ac_perl_destdir.m4,v 1.1 2006-07-17 22:06:43 chu11 Exp $
##*****************************************************************************

# Used almost exclusively so perl install works with rpm builds
AC_DEFUN([AC_PERL_DESTDIR],
[
  AC_MSG_CHECKING(for --with-perl-destdir )
  AC_ARG_WITH(perl-destdir,
     AC_HELP_STRING([--with-perl-destdir=path], 
                    [define perl modules install prefix]),
     [ case "$withval" in
     yes)
         withperlldestdir=yes
         ;;
     no)
         ;;
     *)
         withperlldestdir=yes
         PERL_DESTDIR="$withval"
         ;;
     esac ]
  )
  AC_MSG_RESULT(${withperlldestdir=no})
  AC_SUBST(PERL_DESTDIR)
])
