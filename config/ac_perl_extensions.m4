##*****************************************************************************
## $Id: ac_perl_extensions.m4,v 1.1 2009-02-19 00:50:17 chu11 Exp $
##*****************************************************************************

# Primarily to work around Suse issue, where only a 32bit
# perl/Config.pm is installed, which leads to only 32 bit Perl
# Extensions building possible.
AC_DEFUN([AC_PERL_EXTENSIONS],
[
  AC_MSG_CHECKING(for --with-perl-extensions)
  AC_ARG_WITH(perl-extensions,
     AC_HELP_STRING([--with-perl-extensions=], 
                    [enable or disable perl extensions build]),
     [ case "$withval" in
     yes)
         ac_with_perl_extensions=yes
         ;;
     no)
         ac_with_perl_extensions=no
         ;;
     *)
         ac_with_perl_extensions=yes
         ;;
     esac ]
  )
  AC_MSG_RESULT(${ac_with_perl_extensions=yes})
])
