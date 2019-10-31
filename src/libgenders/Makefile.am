##*****************************************************************************
## $Id: Makefile.am,v 1.30 2008-07-21 21:14:13 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

if WITH_GNU_LD
VERSION_SCRIPT = $(srcdir)/genders.map
OTHER_FLAGS = -Wl,--version-script=$(VERSION_SCRIPT)
endif

BUILT_SOURCES         = genders_query_parse.c genders_query.tab.c
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
			genders_query.tab.c \
			genders_util.c

libgenders_la_LIBADD = ../libcommon/libcommon.la

libgenders_la_LDFLAGS = -version-info @LIBGENDERS_VERSION_INFO@ $(OTHER_FLAGS)

# achu: -o option in lex/flex is not portable, use -t and write to stdout
genders_query_parse.c: genders_query.c $(srcdir)/genders_query_parse.l
	$(LEX) -t $(srcdir)/genders_query_parse.l > $(srcdir)/genders_query_parse.c

# achu: -o option in yacc/bison is not portable, use -b instead
genders_query.c: $(srcdir)/genders_query.y
	$(YACC) -d -b genders_query $(srcdir)/genders_query.y

# achu: -o option in yacc/bison is not portable, use -b instead
genders_query.tab.c: $(srcdir)/genders_query.y
	$(YACC) -d -b genders_query $(srcdir)/genders_query.y

CLEANFILES = genders_query_parse.c genders_query.tab.c genders_query.tab.h

EXTRA_DIST = genders.map genders_query_parse.l genders_query.y

../libcommon/libcommon.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:
