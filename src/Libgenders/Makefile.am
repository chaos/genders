##*****************************************************************************
## $Id: Makefile.am,v 1.14 2004-06-08 16:09:39 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Use Perl's ExtUtils::MakeMaker to get autoconf/automake to install
# into the correct location

PERL_INSTALL_PREFIX = @PERL_INSTALL_PREFIX@

all: Makefile.xs
	$(MAKE) -f Makefile.xs

Makefile.xs: Makefile.PL Libgenders.xs Libgenders.pm
	cp $(srcdir)/../libgenders/*.c .
	cp $(srcdir)/../libgenders/*.h .
	$(PERL) $(srcdir)/Makefile.PL PREFIX=$(PERL_INSTALL_PREFIX) MAKEFILE=Makefile.xs FIRST_MAKEFILE=Makefile.xs

test: Makefile.xs 
	$(MAKE) -f Makefile.xs test

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
	rm -f *.c
	rm -f *.h
	rm -f *.o
	rm -f Makefile.xs
	rm -f Makefile.xs.old

EXTRA_DIST = Libgenders.pm Libgenders.xs Makefile.PL typemap
