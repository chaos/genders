#!/bin/ksh
#
# Copyright (C) 2000-2001 Regents of the University of California
# See the DISCLAIMER file distributed with this package.
#

PACKAGE=	genders
VERSION=	0.2
prefix=		/usr/local

top_srcdir=     .
mkinstalldirs=  $(SHELL) $(top_srcdir)/auxdir/mkinstalldirs

all: inst

install:
	install -m 555 -o root -g root pdsh 	$prefix/bin/pdsh

clean:
	rm -f inst a.out core

# DEVELOPER TARGETS - Borrowed from conman 0.1.3 (cdunlap@llnl.gov)

tar \
$(PACKAGE)-$(VERSION).tgz: VERSION
	@if test -z "$(PACKAGE)"; then echo "PACKAGE undefined"; exit 1; fi; \
	if test -z "$(VERSION)"; then echo "VERSION undefined"; exit 1; fi; \
	test -z "$$tag" && tag=`echo $(PACKAGE)-$(VERSION) | tr '.' '-'`; \
	ver=`cvs co -r $$tag -p $(PACKAGE)/VERSION 2>/dev/null | \
	  sed -ne 's/.*-\(.*\)/\1/p'`; \
	if test -z "$$ver"; then echo "VERSION ($$tag) undefined"; exit 1; fi; \
	tmp=$${TMPDIR-/tmp}/tmp-$(PACKAGE)-$$$$; \
	name=$(PACKAGE)-$$ver; \
	dir=$$tmp/$$name; \
	echo "creating $$name.tgz ($$tag)"; \
	$(mkinstalldirs) $$tmp >/dev/null; \
	cvs export -r $$tag -d $$dir $(PACKAGE) >/dev/null && \
	  (cd $$tmp; tar cf - $$name) | gzip -c9 > $$name.tgz; \
	rm -rf $$tmp

rpm: $(PACKAGE).spec $(PACKAGE)-$(VERSION).tgz
	@if test -z "$(PACKAGE)"; then echo "PACKAGE undefined"; exit 1; fi; \
	if test -z "$(VERSION)"; then echo "VERSION undefined"; exit 1; fi; \
	tmp=$${TMPDIR-/tmp}/tmp-$(PACKAGE)-$$$$; \
	test -f $(PACKAGE)-$(VERSION).tgz || exit 0; \
	for d in BUILD RPMS SOURCES SPECS SRPMS TMP; do \
	  $(mkinstalldirs) $$tmp/$$d >/dev/null; \
	done; \
	cp -p $(PACKAGE).spec $$tmp/SPECS; \
	cp -p $(PACKAGE)-$(VERSION).tgz $$tmp/SOURCES; \
	echo "creating $(PACKAGE)-$(VERSION) rpm"; \
	rpm --showrc | egrep "_(gpg|pgp)_name" >/dev/null && sign="--sign"; \
	rpm -ba --define "_tmppath $$tmp/TMP" --define "_topdir $$tmp" \
	  $$sign --quiet $$tmp/SPECS/$(PACKAGE).spec && \
	    cp -p $$tmp/RPMS/*/$(PACKAGE)-$(VERSION)*.*.rpm \
	      $$tmp/SRPMS/$(PACKAGE)-$(VERSION)*.src.rpm ./; \
	rm -rf $$tmp
