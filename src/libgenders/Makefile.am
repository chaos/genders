##*****************************************************************************
## $Id: Makefile.am,v 1.24 2005-01-10 16:45:35 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

if WITH_GNU_LD
VERSION_SCRIPT = $(srcdir)/genders.map
OTHER_FLAGS = -Wl,--version-script=$(VERSION_SCRIPT)
endif

BUILT_SOURCES         = genders_query_parse.c genders_query.c
include_HEADERS       = genders.h
noinst_HEADERS        = genders_common.h genders_parsing.h fd.h hash.h \
                        hostlist.h list.h thread.h
lib_LTLIBRARIES       = libgenders.la
libgenders_la_CFLAGS  = -D_REENTRANT
libgenders_la_SOURCES = genders.c genders_common.c genders_parsing.c \
                        genders_query_parse.c genders_query.c \
	                fd.c list.c hash.c thread.c hostlist.c
libgenders_la_LDFLAGS = -version-info @LIBGENDERS_VERSION_INFO@ $(OTHER_FLAGS)

genders_query_parse.c: genders_query.c $(srcdir)/genders_query_parse.l
	$(LEX) -ogenders_query_parse.c $(srcdir)/genders_query_parse.l

genders_query.c: $(srcdir)/genders_query.y
	$(YACC) -d -o genders_query.c $(srcdir)/genders_query.y

CLEANFILES = genders_query_parse.c genders_query.c genders_query.h

EXTRA_DIST = genders.map genders_query.h genders_query_parse.l genders_query.y
