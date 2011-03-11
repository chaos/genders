##*****************************************************************************
## $Id: Makefile.am,v 1.25 2009-08-12 23:35:19 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Use Perl's ExtUtils::MakeMaker to get autoconf/automake to install
# into the correct location

# DESTDIR is usually set during make install time, not configure or
# make time, so we work around it with the --with-extension-destdir
# autoconf option.

PERL_DESTDIR = @EXTENSION_DESTDIR@

PERL_INC = "-I$(srcdir)/../../../../config -I$(srcdir)/../../../libgenders"

PERL_LIBS = "-L$(srcdir)/../../../libgenders/.libs -lgenders"

MAKEMAKERFLAGS = INSTALLDIRS=$(PERL_ARCH_INSTALL) \
		 DESTDIR=$(PERL_DESTDIR) \
		 PREFIX=$(prefix) \
		 INC=$(PERL_INC) \
		 LIBS=$(PERL_LIBS) \
		 GENDERS_LIBDIR=$(libdir) \
		 MAKEFILE=Makefile.xs \
		 FIRST_MAKEFILE=Makefile.xs

if WITH_PERL_EXTENSIONS
all: Makefile.xs
	$(MAKE) -f Makefile.xs

Makefile.xs: Makefile.PL Libgenders.xs Libgenders.pm
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
	$(PERL) Makefile.PL $(MAKEMAKERFLAGS)

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
endif

EXTRA_DIST = Libgenders.pm Libgenders.xs Makefile.PL typemap
