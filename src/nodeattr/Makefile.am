##*****************************************************************************
## $Id: Makefile.am,v 1.3 2003-11-07 19:25:27 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

CPPFLAGS = -I../libgenders -I../../config/

bin_PROGRAMS = nodeattr

nodeattr_SOURCES = nodeattr.c hostlist.c hostlist.h

nodeattr_LDADD = ../libgenders/libgenders.la

../libgenders/libgenders.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:
