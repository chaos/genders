##*****************************************************************************
## $Id: Makefile.am,v 1.11 2007-01-09 17:21:19 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

include config/Make-rpm.mk

DISTCHECK_CONFIGURE_FLAGS = --with-perl-destdir=$$dc_install_base

SUBDIRS = \
        src \
        compat \
        man

EXTRA_DIST = META DISCLAIMER TUTORIAL genders.sample
