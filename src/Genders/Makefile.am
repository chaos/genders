##*****************************************************************************
## $Id: Makefile.am,v 1.30 2007-01-09 17:21:20 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

INSTALLSITEARCH=`$(PERL) -e 'use Config; $$T=$$Config{installsitearch}; $$T=~/^\Q$$Config{siteprefix}\E\/(.+)/; print $$1;'`

GENDERSPMFILES = Genders.pm
GENDERSPMFILESdir = $(prefix)/$(INSTALLSITEARCH)
GENDERSPMFILES_DATA = $(GENDERSPMFILES)

Genders.$(PERLMAN3EXT):
# Hacks to make distcheck work
	if ! test -a Genders.pm; then \
		cp $(srcdir)/Genders.pm .; \
	fi
	$(POD2MAN) Genders.pm Genders.$(PERLMAN3EXT)

man3_MANS = Genders.$(PERLMAN3EXT)

install-data-hook:
	chmod 444 $(DESTDIR)/$(prefix)/$(INSTALLSITEARCH)/Genders.pm

EXTRA_DIST = Genders.pm
