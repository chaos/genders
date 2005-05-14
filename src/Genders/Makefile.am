##*****************************************************************************
## $Id: Makefile.am,v 1.27 2005-05-14 02:43:22 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

INSTALLSITEARCH=`perl -e 'use Config; $$T=$$Config{installsitearch}; $$T=~/\/usr\/lib.*\/(.*)/; print $$1;'`

GENDERSPMFILES = Genders.pm
GENDERSPMFILESdir = $(libdir)/$(INSTALLSITEARCH)
GENDERSPMFILES_DATA = $(GENDERSPMFILES)

Genders.3pm:
	$(POD2MAN) Genders.pm Genders.3pm

man3_MANS = Genders.3pm

install-data-hook:
	chmod 444 $(DESTDIR)/$(libdir)/$(INSTALLSITEARCH)/Genders.pm

EXTRA_DIST = Genders.pm
