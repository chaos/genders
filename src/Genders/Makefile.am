##*****************************************************************************
## $Id: Makefile.am,v 1.23 2005-05-13 22:42:46 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

INSTALLSITEARCH=`perl -e 'use Config; $$T=$$Config{installsitearch}; $$T=~/\/usr\/lib(.*)/; print $$1;'`

GENDERSPMFILES = Genders.pm
EXTRA_DIST = $(GENDERSPMFILES)
GENDERSPMFILESdir = $(libdir)/$(INSTALLSITEARCH)
GENDERSPMFILES_DATA = $(GENDERSPMFILES)

install-data-hook:
	chmod 444 $(DESTDIR)/$(libdir)/$(INSTALLSITEARCH)/Genders.pm
