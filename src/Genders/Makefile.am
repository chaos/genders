##*****************************************************************************
## $Id: Makefile.am,v 1.29 2005-05-14 04:59:56 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

INSTALLSITEARCH=`perl -e 'use Config; $$T=$$Config{installsitearch}; $$T=~/\/usr\/(lib|lib64)\/(.*)/; print $$2;'`

GENDERSPMFILES = Genders.pm
GENDERSPMFILESdir = $(libdir)/$(INSTALLSITEARCH)
GENDERSPMFILES_DATA = $(GENDERSPMFILES)

Genders.3pm:
# Hacks to make distcheck work
	if ! test -a Genders.pm; then \
		cp $(srcdir)/Genders.pm .; \
	fi
	$(POD2MAN) Genders.pm Genders.3pm

man3_MANS = Genders.3pm

install-data-hook:
	chmod 444 $(DESTDIR)/$(libdir)/$(INSTALLSITEARCH)/Genders.pm

EXTRA_DIST = Genders.pm
