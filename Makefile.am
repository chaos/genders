##*****************************************************************************
## $Id: Makefile.am,v 1.10 2004-12-29 22:35:40 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

include config/Make-rpm.mk

DISTCHECK_CONFIGURE_FLAGS = --with-perl-install-destdir=$$dc_install_base

SUBDIRS = \
        src \
        compat \
        man

EXTRA_DIST = META DISCLAIMER TUTORIAL genders.sample
