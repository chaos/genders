#
# Copyright (C) 2000 Regents of the University of California
# See the DISCLAIMER file distributed with this package.
#
XFILES=		dist2 dist_all dist_cmp dist_local dist_passwd nodeattr
XDEST=		/var/dist/admin/scripts

LIBFILES=	gendlib.pl hostlist.pl
LIBDEST=	/var/dist/admin/lib

MAN1FILES=	dist2.1 dist_all.1 dist_cmp.1 dist_local.1 nodeattr.1 inst.1
MAN1DEST=	/usr/local/man/man1

MAN3FILES=	gendlib.3
MAN3DEST=	/usr/local/man/man3

INST=		/usr/local/bin/inst -q -o root -g root

all: inst

install: inst
	@./inst -c -q -o root -g root -m 555 -s inst -d /usr/local/bin
	@for file in ${XFILES}; do \
		$(INST) -m 555 -s $$file -d ${XDEST}; \
	done
	@for file in ${LIBFILES}; do \
		$(INST) -m 444 -s $$file -d ${LIBDEST}; \
	done
	@for file in ${MAN1FILES}; do \
		$(INST) -m 444 -s $$file -d ${MAN1DEST}; \
	done
	@for file in ${MAN3FILES}; do \
		$(INST) -m 444 -s $$file -d ${MAN3DEST}; \
	done
	@$(INST) -m 555 -s bootstrap -d /var/dist

clean:
	rm -f inst
