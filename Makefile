#!/bin/ksh
#
# Copyright (C) 2000-2001 Regents of the University of California
# See the DISCLAIMER file distributed with this package.
#

PACKAGE=	genders
VERSION=	0.2
RELEASE=	2

all: inst

install:
	install -m 555 -o root -g root pdsh 	$prefix/bin/pdsh

clean:
	rm -f inst a.out core

include Make-rpm.mk
