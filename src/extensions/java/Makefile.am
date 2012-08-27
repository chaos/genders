##*****************************************************************************
## $Id: Makefile.am,v 1.12 2006-06-19 21:25:41 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

if WITH_JAVA_EXTENSIONS
GENDERS_JAVA_FILES = \
	$(srcdir)/gov/llnl/lc/chaos/Genders.java \
	$(srcdir)/gov/llnl/lc/chaos/GendersException.java \
	$(srcdir)/gov/llnl/lc/chaos/GendersExceptionOpen.java \
	$(srcdir)/gov/llnl/lc/chaos/GendersExceptionRead.java \
	$(srcdir)/gov/llnl/lc/chaos/GendersExceptionParse.java \
	$(srcdir)/gov/llnl/lc/chaos/GendersExceptionParameters.java \
	$(srcdir)/gov/llnl/lc/chaos/GendersExceptionNotfound.java \
	$(srcdir)/gov/llnl/lc/chaos/GendersExceptionSyntax.java \
	$(srcdir)/gov/llnl/lc/chaos/GendersExceptionInternal.java

noinst_HEADERS  = src/Gendersjni.h
lib_LTLIBRARIES = src/libGendersjni.la

src_libGendersjni_la_CXXFLAGS = -D_REENTRANT \
			    -I $(srcdir)/../../libgenders/

src_libGendersjni_la_SOURCES = src/Gendersjni.c

src_libGendersjni_la_LIBADD = ../../libgenders/libgenders.la

src_libGendersjni_la_LDFLAGS = -version-info @LIBGENDERSJNI_VERSION_INFO@ $(OTHER_FLAGS)

all: $(GENDERS_JAVA_FILES)
	$(JAVAC) $(GENDERS_JAVA_FILES)
	$(JAR) cvf $(builddir)/gov/llnl/lc/chaos/Genders.jar $(builddir)/gov/llnl/lc/chaos/*.class
	$(JAVADOC) -d javadoc $(GENDERS_JAVA_FILES)

genders_header: all
	$(JAVAH) -o src/Gendersjni.h gov.llnl.lc.chaos.Genders

install-data-local: all
	$(INSTALL) -m 755 -d $(DESTDIR)$(datadir)/java
	$(INSTALL) -m 644 $(builddir)/gov/llnl/lc/chaos/Genders.jar $(DESTDIR)$(datadir)/java
	$(INSTALL) -m 755 -d $(DESTDIR)$(docdir)-$(VERSION)-javadoc/
# achu: Not sure of recursive install command, is there one?
	cp -r javadoc/* $(DESTDIR)$(docdir)-$(VERSION)-javadoc

uninstall:
	rm -f $(DESTDIR)$(datadir)/java/Genders.jar
	rm -rf $(DESTDIR)$(docdir)-$(VERSION)-javadoc/*

test:
	$(JAVAC) -classpath . GendersTest/GendersTest.java
	$(JAVA) -Djava.library.path=src/.libs -classpath ".:GendersTest" GendersTest

clean-local:
	rm -f gov/llnl/lc/chaos/*.class
	rm -f gov/llnl/lc/chaos/*.jar
	rm -f GendersTest/*.class
	rm -rf javadoc/*

EXTRA_DIST = \
	$(GENDERS_JAVA_FILES) \
	GendersTest/GendersTest.java \
	gov/llnl/lc/chaos/MANIFEST.MF

../../libgenders/libgenders.la: force-dependency-check
	@cd `dirname $@` && make `basename $@`

force-dependency-check:

endif
