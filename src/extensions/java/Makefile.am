##*****************************************************************************
## $Id: Makefile.am,v 1.12 2006-06-19 21:25:41 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

if WITH_JAVA_EXTENSIONS
GENDERS_JAVA_FILES = \
	gov/llnl/lc/chaos/Genders.java \
	gov/llnl/lc/chaos/GendersException.java \
	gov/llnl/lc/chaos/GendersExceptionOpen.java \
	gov/llnl/lc/chaos/GendersExceptionRead.java \
	gov/llnl/lc/chaos/GendersExceptionParse.java \
	gov/llnl/lc/chaos/GendersExceptionParameters.java \
	gov/llnl/lc/chaos/GendersExceptionNotfound.java \
	gov/llnl/lc/chaos/GendersExceptionSyntax.java \
	gov/llnl/lc/chaos/GendersExceptionInternal.java

noinst_HEADERS  = src/Gendersjni.h
lib_LTLIBRARIES = src/libGendersjni.la

src_libGendersjni_la_CXXFLAGS = -D_REENTRANT \
			    -I $(srcdir)/../../libgenders/

src_libGendersjni_la_SOURCES = src/Gendersjni.c

src_libGendersjni_la_LIBADD = ../../libgenders/libgenders.la

src_libGendersjni_la_LDFLAGS = -version-info @LIBGENDERSJNI_VERSION_INFO@ $(OTHER_FLAGS)

all:
	$(JAVAC) $(GENDERS_JAVA_FILES)

genders_header:
	$(JAVAH) -o src/Gendersjni.h gov.llnl.lc.chaos.Genders

test:
	$(JAVAC) -classpath . GendersTest/GendersTest.java
	$(JAVA) -Djava.library.path=src/.libs -classpath ".:GendersTest" GendersTest

clean-local:
	rm -f gov/llnl/lc/chaos/*.class
	rm -f GendersTest/*.class
endif
