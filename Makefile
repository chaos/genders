#
# Copyright (C) 2000-2001 Regents of the University of California
# See the DISCLAIMER file distributed with this package.
#

PROJECT=	genders

all: inst
	cd src;	./configure; make 

install:
	install -m 555 -o root -g root inst /usr/local/bin
	cd src; make install

clean:
	rm -f inst a.out core *.rpm *.tgz *.a *.o

include Make-rpm.mk
