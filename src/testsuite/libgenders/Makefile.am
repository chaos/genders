##*****************************************************************************
## $Id: Makefile.am,v 1.2 2005-01-03 17:31:21 achu Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

noinst_PROGRAMS      = genders_test
noinst_HEADERS       = genders_test_corner_case.h \
	               genders_test_database.h \
		       genders_test_functionality.h \
		       genders_test_query_tests.h \
		       genders_testlib.h
genders_test_CFLAGS  = -I../../libgenders -I../../../config/
genders_test_SOURCES = genders_test.c \
                       genders_test_corner_case.c \
                       genders_test_corner_case_tests.c \
                       genders_test_database.c \
		       genders_test_functionality.c \
		       genders_test_query_tests.c \
		       genders_testlib.c
genders_test_LDADD   = ../../libgenders/libgenders.la

../../libgenders/libgenders.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:
