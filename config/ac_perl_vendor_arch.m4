##*****************************************************************************
## $Id: ac_perl_vendor_arch.m4,v 1.1 2009-02-19 00:50:17 chu11 Exp $
##*****************************************************************************

AC_DEFUN([AC_PERL_VENDOR_ARCH],
[
  AC_MSG_CHECKING(for --with-perl-vendor-arch)
  AC_ARG_WITH(perl-vendor-arch,
     AC_HELP_STRING([--with-perl-vendor-arch=], 
                    [enable or disable perl extensions install in vendor arch]),
     [ case "$withval" in
     yes)
         ac_with_perl_vendor_arch=yes
         ;;
     no)
         ac_with_perl_vendor_arch=no
         ;;
     *)
         ac_with_perl_vendor_arch=yes
         ;;
     esac ]
  )
  AC_MSG_RESULT(${ac_with_perl_vendor_arch=no})
])
