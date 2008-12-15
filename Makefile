#!/usr/bin/make -f
# -*- makefile -*-

#    Cifer: Automating classical cipher cracking in C
#    Copyright (C) 2008  Daniel Richman & Simrun Basuita
#
#    Cifer is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Cifer is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Cifer.  If not, see <http://www.gnu.org/licenses/>.

SHELL = /bin/sh

version := $(shell cat VERSION)

CC = gcc
CFLAGS = -Wall -pedantic

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1

INSTALL = install
INSTALL_PROGRAM = $(INSTALL) -m 755
INSTALL_DATA = $(INSTALL) -m 644

srcmandir = manpages/

rm = rm -f
rmdir = rm -rf
mkdir = mkdir -p

objects := $(patsubst %.c,%.o,$(wildcard src/*.c))
headers := $(wildcard src/*.h src/*.inc)

allfiles := $(shell ls)


all : cifer


cifer : $(objects)
	$(CC) $(CFLAGS) -o $@ $(objects)


$(objects) : $(headers)
	$(CC) -Wall -pedantic -DVERSION="\"$(version)\"" $(CFLAGS) -c -o $@ \
	 $(patsubst %.o,%.c,$@)


.PHONY : all \
	\
	install \
	install-man \
	install-bin \
	install-dir-man \
	install-dir-bin \
	\
	uninstall \
	uninstall-man \
	uinstall-bin \
	\
	clean \
	distclean \
	\
	dist


install : install-man install-bin

install-dir-man :
	[ -d $(DESTDIR)$(man1dir) ] || $(mkdir) $(DESTDIR)$(man1dir)

install-dir-bin :
	[ -d $(DESTDIR)$(bindir) ] || $(mkdir) $(DESTDIR)$(bindir)

install-man : install-dir-man
	$(INSTALL_DATA) $(srcmandir)/cifer.1 \
	$(DESTDIR)$(man1dir)/cifer.1
	$(INSTALL_DATA) $(srcmandir)/cifer-dict.1 \
	$(DESTDIR)$(man1dir)/cifer-dict.1

install-bin : all install-dir-bin
	$(INSTALL_PROGRAM) cifer $(DESTDIR)$(bindir)/cifer
	$(INSTALL_PROGRAM) cifer-dict $(DESTDIR)$(bindir)/cifer-dict

uninstall: uninstall-man uninstall-bin

uninstall-man :
	$(rm) $(DESTDIR)$(man1dir)/cifer.1
	$(rm) $(DESTDIR)$(man1dir)/cifer-dict.1

uninstall-bin :
	$(rm) $(DESTDIR)$(bindir)/cifer
	$(rm) $(DESTDIR)$(bindir)/cifer-dict


clean :
	$(rm) cifer $(objects)

distclean : clean


dist : clean
	$(mkdir) cifer-$(version)
	for i in $(allfiles); do \
	  cp --recursive $$i cifer-$(version); \
	done
	
	$(rmdir) cifer-$(version)/copy-header \
	cifer-$(version)/notes \
	cifer-$(version)/tests
	
	tar cf - cifer-$(version) | \
	gzip -9f > \
	cifer-$(version).tar.gz
	
	$(rmdir) cifer-$(version)
