##*****************************************************************************
## $Id: Makefile.am,v 1.33 2009-08-12 21:52:37 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

INSTALLXARCH="install${PERL_ARCH_INSTALL}arch"
XPREFIX="${PERL_ARCH_INSTALL}prefix"
INSTALLARCH=`$(PERL) -e 'use Config; $$T=$$Config{$(INSTALLXARCH)}; $$T=~/^\Q$$Config{$(XPREFIX)}\E\/(.+)/; print $$1;'`

if WITH_PERL_EXTENSIONS
GENDERSPMFILES = Genders.pm
GENDERSPMFILESdir = $(prefix)/$(INSTALLARCH)
GENDERSPMFILES_DATA = $(GENDERSPMFILES)

Genders.$(PERLMAN3EXT):
# Hacks to make distcheck work
	if ! test -a Genders.pm; then \
		cp $(srcdir)/Genders.pm .; \
	fi
	$(POD2MAN) Genders.pm Genders.$(PERLMAN3EXT)

man3_MANS = Genders.$(PERLMAN3EXT)

install-data-hook:
	chmod 444 $(DESTDIR)$(prefix)/$(INSTALLARCH)/Genders.pm
endif

EXTRA_DIST = Genders.pm
