##*****************************************************************************
## $Id: Makefile.am,v 1.1 2006-06-19 21:25:41 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

noinst_HEADERS = hash.h \
		 hostlist.h \
		 list.h \
		 thread.h

noinst_LTLIBRARIES = libcommon.la
libcommon_la_SOURCES = hash.c \
		       hostlist.c \
		       list.c  \
		       thread.c

libcommon_la_CFLAGS = -I../../config
