##*****************************************************************************
## $Id: Makefile.am,v 1.18 2005-04-19 23:47:27 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Use Perl's ExtUtils::MakeMaker to get autoconf/automake to install
# into the correct location

# DESTDIR is usually set during make install time, not configure or
# make time, so this is our work around
PERL_INSTALL_DESTDIR = @PERL_INSTALL_DESTDIR@
PERL_INSTALL_PREFIX = @PERL_INSTALL_PREFIX@

MAKEMAKERFLAGS = INSTALLDIRS=site \
		 SITEPREFIX=$(PERL_INSTALL_PREFIX) \
                 MAKEFILE=Makefile.xs \
		 FIRST_MAKEFILE=Makefile.xs

all: Makefile.xs
	$(MAKE) -f Makefile.xs

Makefile.xs: Makefile.PL
# Hacks to make distcheck work
	if ! test -a Makefile.PL; then \
		cp $(srcdir)/Makefile.PL .; \
	fi
	if ! test -a Genders.pm; then \
		cp $(srcdir)/Genders.pm .; \
	fi
	TEMPINSTALLSITELIB=`perl -e 'use Config; print $$Config{installsitearch}'`; \
	$(PERL) Makefile.PL $(MAKEMAKERFLAGS) INSTALLSITELIB=$(PERL_INSTALL_DESTDIR)$$TEMPINSTALLSITELIB

install: Makefile.xs
	$(MAKE) -f Makefile.xs pure_install

pure_install: Makefile.xs
	$(MAKE) -f Makefile.xs pure_install

install-data-local:
	$(MAKE) -f Makefile.xs pure_install

uninstall:
	$(MAKE) -f Makefile.xs uninstall

clean:
	if test -a Makefile.xs; then $(MAKE) -f Makefile.xs clean; fi
	rm -f Makefile.xs
	rm -f Makefile.xs.old

EXTRA_DIST = Genders.pm Makefile.PL
