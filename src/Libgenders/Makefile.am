##*****************************************************************************
## $Id: Makefile.am,v 1.3 2003-05-19 15:56:38 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# This is a hack to get autoconf, automake, and ExtUtils::MakeMaker
# to work together.  
#
# Currently, it only supports the common MakeMaker options of
#
# make install
# make pure_install
# make test

XS_PREFIX = @XS_PREFIX@

all: Makefile.xs
        $(MAKE) -f Makefile.xs

Makefile.xs: Makefile.PL Libgenders.xs Libgenders.pm
        cp ../libgenders/genders.c .
        cp ../libgenders/genders.h .
        cp ../libgenders/genders_local.h .
        $(PERL) Makefile.PL PREFIX=$(XS_PREFIX) MAKEFILE=Makefile.xs FIRST_MAKEFILE=Makefile.xs

test: Makefile.xs 
        $(MAKE) -f Makefile.xs test

install: Makefile.xs
        $(MAKE) -f Makefile.xs pure_install

pure_install: Makefile.xs
        $(MAKE) -f Makefile.xs pure_install

install-data-local:
        $(MAKE) -f Makefile.xs pure_install

clean: 
        $(MAKE) -f Makefile.xs clean
        rm -f Makefile.xs
        rm -f Makefile.xs.old
        rm -f genders.c
        rm -f genders.h
        rm -f genders_local.h

