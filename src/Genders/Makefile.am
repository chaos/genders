##*****************************************************************************
## $Id: Makefile.am,v 1.16 2004-12-20 16:38:04 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Use Perl's ExtUtils::MakeMaker to get autoconf/automake to install
# into the correct location

# DESTDIR is usually set during make install time, not configure or
# make time, so this is our work around
PERL_INSTALL_DESTDIR = @PERL_INSTALL_DESTDIR@
PERL_INSTALL_PREFIX = @PERL_INSTALL_PREFIX@

all: Makefile.pm
	$(MAKE) -f Makefile.pm

Makefile.pm: Makefile.PL
# Hacks to make distcheck work
	if ! test -a Makefile.PL; then \
		cp $(srcdir)/Makefile.PL .; \
	fi
	if ! test -a Genders.pm; then \
		cp $(srcdir)/Genders.pm .; \
	fi
	TEMPINSTALLSITELIB=`perl -e 'use Config; print $$Config{installsitearch}'`; \
	$(PERL) Makefile.PL PREFIX=$(PERL_INSTALL_PREFIX) MAKEFILE=Makefile.pm FIRST_MAKEFILE=Makefile.pm INSTALLSITELIB=$(PERL_INSTALL_DESTDIR)$$TEMPINSTALLSITELIB

install: Makefile.pm
	$(MAKE) -f Makefile.pm pure_install

pure_install: Makefile.pm
	$(MAKE) -f Makefile.pm pure_install

install-data-local:
	$(MAKE) -f Makefile.pm pure_install

uninstall:
	$(MAKE) -f Makefile.pm uninstall

clean:
	if test -a Makefile.pm; then $(MAKE) -f Makefile.pm clean; fi
	rm -f Makefile.pm
	rm -f Makefile.pm.old

EXTRA_DIST = Genders.pm Makefile.PL
