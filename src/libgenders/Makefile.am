##*****************************************************************************
## $Id: Makefile.am,v 1.6 2003-12-31 18:38:09 achu Exp $
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

include_HEADERS       = genders.h
lib_LTLIBRARIES       = libgenders.la
libgenders_la_SOURCES = genders.c fd.c list.c
libgenders_la_LDFLAGS   = -version-info $(current):$(rev):$(age)
