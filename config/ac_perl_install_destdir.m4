##*****************************************************************************
## $Id: ac_perl_install_destdir.m4,v 1.1 2005-04-19 23:47:27 achu Exp $
##*****************************************************************************

# - Used almost exclusively so perl install works with rpm builds
AC_DEFUN([AC_PERL_INSTALL_DESTDIR],
[
  AC_MSG_CHECKING(for --with-perl-install-destdir )
  AC_ARG_WITH(perl-install-destdir,
     AC_HELP_STRING([--with-perl-install-destdir=path], 
                    [define perl modules install prefix]),
     [ case "$withval" in
     yes)
         withperllinstalldestdir=yes
         ;;
     no)
         ;;
     *)
         withperllinstalldestdir=yes
         PERL_INSTALL_DESTDIR="$withval"
         ;;
     esac ]
  )
  AC_MSG_RESULT(${withperllinstalldestdir=no})
  if test -z "$PERL_INSTALL_DESTDIR"; then
     PERL_INSTALL_PREFIX='${prefix}'
  else
     PERL_INSTALL_PREFIX='${PERL_INSTALL_DESTDIR}${prefix}'
  fi
  AC_SUBST(PERL_INSTALL_DESTDIR)
  AC_SUBST(PERL_INSTALL_PREFIX)
])
