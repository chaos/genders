##*****************************************************************************
## $Id: Makefile.am,v 1.5 2006-06-19 21:25:44 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

bin_PROGRAMS     = nodeattr
nodeattr_CFLAGS  = -I $(srcdir)/../../config \
		   -I $(srcdir)/../libcommon \
		   -I $(srcdir)/../libgenders 
nodeattr_SOURCES = nodeattr.c 
nodeattr_LDADD   = ../libcommon/libcommon.la \
		   ../libgenders/libgenders.la

../libcommon/libcommon.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

../libgenders/libgenders.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:
