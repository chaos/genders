##*****************************************************************************
## $Id: Makefile.am,v 1.17 2004-12-14 21:39:24 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Use Perl's ExtUtils::MakeMaker to get autoconf/automake to install
# into the correct location

# DESTDIR is usually set during make install time, not configure or
# make time, so this is our work around
PERL_INSTALL_PREFIX = @PERL_INSTALL_PREFIX@

all: Makefile.xs
	$(MAKE) -f Makefile.xs

Makefile.xs: Makefile.PL Libgenders.xs Libgenders.pm
	cp $(srcdir)/../libgenders/*.c .
	cp $(srcdir)/../libgenders/*.h .
	cp $(srcdir)/../libgenders/*.l .
	cp $(srcdir)/../libgenders/*.y .
	if ! test -a genders_parse.c; then \
		$(LEX) -ogenders_parse.c genders_parse.l; \
	fi
	if ! test -a genders_query.c; then \
		$(YACC) -d -ogenders_query.c genders_query.y; \
	fi
# Hacks to make distcheck work
	if ! test -a Makefile.PL; then \
		cp $(srcdir)/Makefile.PL .; \
	fi
	if ! test -a typemap; then \
		cp $(srcdir)/typemap .; \
	fi
	if ! test -a Libgenders.pm; then \
		cp $(srcdir)/Libgenders.pm .; \
	fi
	if ! test -a Libgenders.xs; then \
		cp $(srcdir)/Libgenders.xs .; \
	fi
	$(PERL) Makefile.PL PREFIX=$(PERL_INSTALL_PREFIX)$(prefix) MAKEFILE=Makefile.xs FIRST_MAKEFILE=Makefile.xs

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
	if test -a Makefile.xs; then \
		$(MAKE) -f Makefile.xs clean; \
	fi
	rm -f *.c
	rm -f *.h
	rm -f *.l
	rm -f *.y
	rm -f *.o
	rm -f Makefile.xs
	rm -f Makefile.xs.old

EXTRA_DIST = Libgenders.pm Libgenders.xs Makefile.PL typemap
