##*****************************************************************************
## $Id: Makefile.am,v 1.9 2004-10-27 01:42:53 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

include config/Make-rpm.mk

DISTCHECK_CONFIGURE_FLAGS = --with-perl-install-prefix=$$dc_install_base

SUBDIRS = \
        src \
        compat \
        man

EXTRA_DIST = META DISCLAIMER TUTORIAL genders.sample
