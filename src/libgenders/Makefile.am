##*****************************************************************************
## $Id: Makefile.am,v 1.15 2004-06-07 20:14:37 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Update this information before a public release only!
# 
# If library source code has changed at all increment rev
# (current:rev:age) = (current:rev+1:age)
#
# If any interfaces have been added,removed, or changed increment current
#  and set rev = 0;
#
# If any interfaces have been added increment age
#
# If any interfaces have been removed, set age = 0
#
current = 0
rev     = 0
age     = 0

if WITH_GNU_LD
VERSION_SCRIPT = genders.map
OTHER_FLAGS = -Wl,--version-script=$(VERSION_SCRIPT)
endif

include_HEADERS       = genders.h
noinst_HEADERS        = genders_common.h fd.h hash.h hostlist.h list.h thread.h
lib_LTLIBRARIES       = libgenders.la
libgenders_la_CFLAGS  = -D_REENTRANT
libgenders_la_SOURCES = genders.c genders_common.c fd.c \
	                list.c hash.c thread.c hostlist.c
libgenders_la_LDFLAGS = -version-info $(current):$(rev):$(age) $(OTHER_FLAGS)
EXTRA_DIST            = genders.map
