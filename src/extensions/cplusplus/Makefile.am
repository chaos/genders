##*****************************************************************************
## $Id: Makefile.am,v 1.30 2008-07-21 21:14:13 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

if WITH_CPLUSPLUS_EXTENSIONS
include_HEADERS       = gendersplusplus.hpp
lib_LTLIBRARIES       = libgendersplusplus.la

libgendersplusplus_la_CXXFLAGS = -D_REENTRANT \
				 -I $(srcdir)/../../libgenders/

libgendersplusplus_la_SOURCES = gendersplusplus.cpp

libgendersplusplus_la_LIBADD = ../../libgenders/libgenders.la

libgendersplusplus_la_LDFLAGS = -version-info @LIBGENDERSPLUSPLUS_VERSION_INFO@ $(OTHER_FLAGS)

../../libgenders/libgenders.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:

endif
