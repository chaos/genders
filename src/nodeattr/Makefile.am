##*****************************************************************************
## $Id: Makefile.am,v 1.4 2004-06-07 20:14:37 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

bin_PROGRAMS     = nodeattr
nodeattr_CFLAGS  = -I../libgenders -I../../config/
nodeattr_SOURCES = nodeattr.c hostlist.c hostlist.h
nodeattr_LDADD   = ../libgenders/libgenders.la

../libgenders/libgenders.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:
