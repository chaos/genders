##*****************************************************************************
## $Id: Makefile.am,v 1.11 2004-01-31 02:22:53 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Use Perl's ExtUtils::MakeMaker to get autoconf/automake to install
# into the correct location

PERL_INSTALL_PREFIX = @PERL_INSTALL_PREFIX@

all: Makefile.xs
	$(MAKE) -f Makefile.xs

Makefile.xs: Makefile.PL Libgenders.xs Libgenders.pm
	cp ../libgenders/genders.c .
	cp ../libgenders/genders.h .
	cp ../libgenders/list.c .
	cp ../libgenders/list.h .
	cp ../libgenders/fd.h .
	cp ../libgenders/fd.c .
	cp ../libgenders/hostlist.c .
	cp ../libgenders/hostlist.h .
	$(PERL) Makefile.PL PREFIX=$(PERL_INSTALL_PREFIX) MAKEFILE=Makefile.xs FIRST_MAKEFILE=Makefile.xs

test: Makefile.xs 
	$(MAKE) -f Makefile.xs test

install: Makefile.xs
	$(MAKE) -f Makefile.xs pure_install

pure_install: Makefile.xs
	$(MAKE) -f Makefile.xs pure_install

install-data-local:
	$(MAKE) -f Makefile.xs pure_install

clean: 
	if test -a Makefile.xs; then $(MAKE) -f Makefile.xs clean; fi
	rm -f genders.*
	rm -f list.*
	rm -f fd.*  
	rm -f hostlist.*
	rm -f Makefile.xs
	rm -f Makefile.xs.old
