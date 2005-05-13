##*****************************************************************************
## $Id: Makefile.am,v 1.21 2005-05-13 22:07:58 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

INSTALLSITEARCH=`perl -e 'use Config; print $$Config{installsitearch}'`

GENDERSPMFILES = Genders.pm
EXTRA_DIST = $(GENDERSPMFILES)
GENDERSPMFILESdir = $(INSTALLSITEARCH)
GENDERSPMFILES_DATA = $(GENDERSPMFILES)

install-data-hook:
	chmod 444 $(DESTDIR)/$(INSTALLSITEARCH)/Genders.pm
