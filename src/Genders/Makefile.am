##*****************************************************************************
## $Id: Makefile.am,v 1.12 2004-06-11 18:10:41 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Use Perl's ExtUtils::MakeMaker to get autoconf/automake to install
# into the correct location

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
	$(PERL) Makefile.PL PREFIX=$(PERL_INSTALL_PREFIX) MAKEFILE=Makefile.pm FIRST_MAKEFILE=Makefile.pm

install: Makefile.pm
	$(MAKE) -f Makefile.pm pure_install

pure_install: Makefile.pm
	$(MAKE) -f Makefile.pm pure_install

install-data-local:
	$(MAKE) -f Makefile.pm pure_install

clean:
	if test -a Makefile.pm; then $(MAKE) -f Makefile.pm clean; fi
	rm -f Makefile.pm
	rm -f Makefile.pm.old

EXTRA_DIST = Genders.pm Makefile.PL
