#
# Copyright (C) 2000-2001 Regents of the University of California
# See the DISCLAIMER file distributed with this package.
#

PROJECT=	genders
PACKAGE=        genders

CFLAGS = -Wall -g

all: inst
	cd src;	./configure; make 
	cd src/libgenders/Libgenders; perl Makefile.PL; make

install:
	install -m 555 -o root -g root inst /usr/local/bin
	cd src; make install
	cd src/libgenders/Libgenders; make install

clean:
	rm -f inst a.out core *.rpm *.tgz *.a *.o
	cd src; make clean
	cd src/libgenders/Libgenders; make clean

include Make-rpm.mk
