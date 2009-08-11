##*****************************************************************************
## $Id: Makefile.am,v 1.32 2009-08-11 16:56:59 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

INSTALLSITEARCH=`$(PERL) -e 'use Config; $$T=$$Config{installsitearch}; print $$T;'`

if WITH_PERL_EXTENSIONS
GENDERSPMFILES = Genders.pm
GENDERSPMFILESdir = $(INSTALLSITEARCH)
GENDERSPMFILES_DATA = $(GENDERSPMFILES)

Genders.$(PERLMAN3EXT):
# Hacks to make distcheck work
	if ! test -a Genders.pm; then \
		cp $(srcdir)/Genders.pm .; \
	fi
	$(POD2MAN) Genders.pm Genders.$(PERLMAN3EXT)

man3_MANS = Genders.$(PERLMAN3EXT)

install-data-hook:
	chmod 444 $(DESTDIR)/$(INSTALLSITEARCH)/Genders.pm
endif

EXTRA_DIST = Genders.pm
