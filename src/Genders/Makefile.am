##*****************************************************************************
## $Id: Makefile.am,v 1.1 2003-05-14 00:08:51 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# We will cheat and use ExtUtils::MakeMaker to install this
# Perl Module instead of doing some whacked out autoconf

GENDERS_PM_PREFIX = @GENDERS_PM_PREFIX@

all: Makefile.pm
	$(MAKE) -f Makefile.pm

Makefile.pm: Makefile.PL
	$(PERL) Makefile.PL PREFIX=$(GENDERS_PM_PREFIX) MAKEFILE=Makefile.pm FIRST_MAKEFILE=\Makefile.pm

install: Makefile.pm
	$(MAKE) -f Makefile.pm install

pure_install: Makefile.pm
	$(MAKE) -f Makefile.pm pure_install

install-data-local:
	$(MAKE) -f Makefile.pm pure_install

clean:
	$(MAKE) -f Makefile.pm clean
	rm -f Makefile.pm
	rm -f Makefile.pm.old
