##*****************************************************************************
## $Id: Makefile.am,v 1.8 2004-06-11 18:23:31 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

include config/Make-rpm.mk

DISTCHECK_CONFIGURE_FLAGS = --with-perl-install-prefix=$$dc_install_base

SUBDIRS = \
        src \
        compat \
        man

EXTRA_DIST = META
