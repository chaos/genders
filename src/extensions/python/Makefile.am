##*****************************************************************************
## $Id: Makefile.am,v 1.25 2009-08-12 23:35:19 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

# Use Python's distutils to make/install everything into the right place.

# DESTDIR is usually set during make install time, not configure or
# make time, so we work around it with the --with-extension-destdir
# autoconf option.

PYTHON_DESTDIR = @EXTENSION_DESTDIR@

if WITH_PYTHON_EXTENSIONS
all: genderssetup.py libgendersmodule.c genders.py
	$(PYTHON) genderssetup.py build

install:
	$(PYTHON) genderssetup.py install --prefix=$(PYTHON_DESTDIR)$(prefix) --exec-prefix=$(PYTHON_DESTDIR)$(exec_prefix)

pure_install:
	$(PYTHON) genderssetup.py install --prefix=$(PYTHON_DESTDIR)$(prefix) --exec-prefix=$(PYTHON_DESTDIR)$(exec_prefix)

install-data-local:
	$(PYTHON) genderssetup.py install --prefix=$(PYTHON_DESTDIR)$(prefix) --exec-prefix=$(PYTHON_DESTDIR)$(exec_prefix)

clean: 
	rm -rf build

endif

EXTRA_DIST = genderssetup.py libgendersmodule.c genders.py
