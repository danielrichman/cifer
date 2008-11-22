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

CC = gcc
CFLAGS = -Wall

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

rm = rm -rf
mkdir = mkdir -p

objects := $(patsubst %.c,%.o,$(wildcard src/*.c))
headers := $(wildcard src/*.h src/*.i)


cifer : $(objects)
	$(CC) $(CFLAGS) -o $@ $(objects)


src/affine.o                    : $(headers)
src/bacon.o                     : $(headers)
src/ciphers.o                   : $(headers)
src/columnar_transposition.o    : $(headers)
src/command.o                   : $(headers)
src/dictionary.o                : $(headers)
src/frequency_analysis.o        : $(headers)
src/frequency_data.o            : $(headers)
src/interface.o                 : $(headers)
src/io.o                        : $(headers)
src/keyword.o                   : $(headers)
src/main.o                      : $(headers)
src/polybius.o                  : $(headers)
src/rf.o                        : $(headers)
src/shell.o                     : $(headers)
src/urandom_access.o            : $(headers)
src/utility.o                   : $(headers)
src/vigenere.o                  : $(headers)
src/vowel_mash.o                : $(headers)


.PHONY : all \
	\
	dist \
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


all : cifer


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


uninstall: uninstall-man uninstall-bin

uninstall-man :
	$(rm) $(DESTDIR)$(man1dir)/cifer.1
	$(rm) $(DESTDIR)$(man1dir)/cifer-dict.1

uninstall-bin :
	$(rm) $(DESTDIR)$(bindir)/cifer


clean :
	$(rm) $(bin_all_name) $(objects)

distclean : clean


dist :
