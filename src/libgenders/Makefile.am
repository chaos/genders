##*****************************************************************************
## $Id: Makefile.am,v 1.27 2006-06-19 21:25:42 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

if WITH_GNU_LD
VERSION_SCRIPT = $(srcdir)/genders.map
OTHER_FLAGS = -Wl,--version-script=$(VERSION_SCRIPT)
endif

BUILT_SOURCES         = genders_query_parse.c genders_query.c
include_HEADERS       = genders.h
noinst_HEADERS        = genders_api.h \
			genders_constants.h \
			genders_parsing.h \
			genders_util.h

lib_LTLIBRARIES       = libgenders.la
libgenders_la_CFLAGS  = -D_REENTRANT \
			-I $(srcdir)/../libcommon
libgenders_la_SOURCES = genders.c \
			genders_parsing.c \
                        genders_query_parse.c \
			genders_query.c \
			genders_util.c

libgenders_la_LIBADD = ../libcommon/libcommon.la

libgenders_la_LDFLAGS = -version-info @LIBGENDERS_VERSION_INFO@ $(OTHER_FLAGS)

genders_query_parse.c: genders_query.c $(srcdir)/genders_query_parse.l
	$(LEX) -ogenders_query_parse.c $(srcdir)/genders_query_parse.l

genders_query.c: $(srcdir)/genders_query.y
	$(YACC) -d -o genders_query.c $(srcdir)/genders_query.y

CLEANFILES = genders_query_parse.c genders_query.c genders_query.h

EXTRA_DIST = genders.map genders_query.h genders_query_parse.l genders_query.y

../libcommon/libcommon.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:
