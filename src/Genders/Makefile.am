##*****************************************************************************
## $Id: Makefile.am,v 1.22 2005-05-13 22:39:34 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

INSTALLSITEARCH=`perl -e 'use Config; $$T=$$Config{installsitearch}; $$T=~/\/usr\/lib(.*)/; print $$1;'`

GENDERSPMFILES = Genders.pm
EXTRA_DIST = $(GENDERSPMFILES)
GENDERSPMFILESdir = $(libdir)/$(INSTALLSITEARCH)
GENDERSPMFILES_DATA = $(GENDERSPMFILES)

install-data-hook:
	chmod 444 $(libdir)/$(INSTALLSITEARCH)/Genders.pm
