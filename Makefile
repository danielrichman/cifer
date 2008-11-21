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

mkdir = mkdir

opt_lvl = 2

bin_norm_name      = cifer
bin_debug_name     = cifer-debug
bin_opt_name       = cifer-opt
bin_opt_debug_name = cifer-opt-debug
bin_all_name       = $(bin_norm_name) \
	$(bin_debug_name) \
	$(bin_opt_name) \
	$(bin_opt_debug_name)

cflags_norm  = 
cflags_debug = -pg -g
cflags_opt   = -O$(opt_lvl)
cflags_opt_debug = $(cflags_opt) -pg -g

objects = src/actions.o \
	src/affine.o \
	src/bacon.o \
	src/ciphers.o \
	src/columnar_transposition.o \
	src/command.o \
	src/dictionary.o \
	src/frequency_analysis.o \
	src/frequency_data.o \
	src/interface.o \
	src/io.o \
	src/keyword.o \
	src/main.o \
	src/polybius.o \
	src/rf.o \
	src/shell.o \
	src/utility.o \
	src/vigenere.o \
	src/vowel_mash.o

cfiles = src/actions.c \
	src/affine.c \
	src/bacon.c \
	src/ciphers.c \
	src/columnar_transposition.c \
	src/command.c \
	src/dictionary.c \
	src/frequency_analysis.c \
	src/frequency_data.c \
	src/interface.c \
	src/io.c \
	src/keyword.c \
	src/main.c \
	src/polybius.c \
	src/rf.c \
	src/shell.c \
	src/utility.c \
	src/vigenere.c \
	src/vowel_mash.c

headers = src/actions.h \
	src/affine.h \
	src/bacon.h \
	src/ciphers.h \
	src/columnar_transposition.h \
	src/command.h \
	src/command_info.h \
	src/dictionary.h \
	src/frequency_analysis.h \
	src/frequency_data.h \
	src/interface.h \
	src/io.h \
	src/keyword.h \
	src/macros.h \
	src/polybius.h \
	src/settings.h \
	src/shell.h \
	src/stdinc.h \
	src/utility.h \
	src/vigenere.h \
	src/vowel_mash.h \
	src/utility.i \
	src/command.i

$(bin_norm_name) : $(objects)
	$(CC) $(cflags_norm) $(CFLAGS)       -o $@ $(objects)

$(bin_debug_name) : $(cfiles)
	$(CC) $(cflags_debug) $(CFLAGS)      -o $@ $(cfiles)

$(bin_opt_name) : $(cfiles)
	$(CC) $(cflags_opt) $(CFLAGS)        -o $@ $(cfiles)

$(bin_opt_debug_name) : $(cfiles)
	$(CC) $(cflags_opt_debug) $(CFLAGS)  -o $@ $(cfiles)

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
	install-norm \
	install-debug \
	install-opt \
	install-opt-debug \
	install-bin-norm \
	install-bin-debug \
	install-bin-opt \
	install-bin-opt-debug \
	\
	uninstall \
	uninstall-man \
	uninstall-norm \
	uninstall-debug \
	uninstall-opt \
	uninstall-opt-debug \
	uninstall-bin-norm \
	uninstall-bin-debug \
	uninstall-bin-opt \
	uninstall-bin-opt-debug \
	\
	clean \
	distclean \
	\
	dist

all : $(bin_norm_name)


install : install-norm

install-man :
	$(INSTALL_DATA) $(srcmandir)/cifer.1 \
	$(DESTDIR)$(man1dir)/cifer.1
	$(INSTALL_DATA) $(srcmandir)/cifer-dict.1 \
	$(DESTDIR)$(man1dir)/cifer-dict.1

install-norm : install-bin-norm install-man

install-debug : install-bin-debug install-man

install-opt : install-bin-opt install-man

install-opt-debug : install-bin-opt-debug install-man

install-bin-norm : $(bin_norm_name)
	$(INSTALL_PROGRAM) $(bin_norm_name) $(DESTDIR)$(bindir)/$(bin_norm_name)

install-bin-debug : $(bin_debug_name)
	$(INSTALL_PROGRAM) $(bin_debug_name) $(DESTDIR)$(bindir)/$(bin_debug_name)

install-bin-opt : $(bin_opt_name)
	$(INSTALL_PROGRAM) $(bin_opt_name) $(DESTDIR)$(bindir)/$(bin_opt_name)

install-bin-opt-debug : $(bin_opt_debug_name)
	$(INSTALL_PROGRAM) $(bin_opt_debug_name) $(DESTDIR)$(bindir)/$(bin_opt_debug_name)


uninstall: uninstall-norm

uninstall-man :
	$(rm) $(DESTDIR)$(man1dir)/cifer.1
	$(rm) $(DESTDIR)$(man1dir)/cifer-dict.1

uninstall-norm : uninstall-bin-norm uninstall-man

uninstall-debug : uninstall-bin-debug uninstall-man

uninstall-opt : uninstall-bin-opt uninstall-man

uninstall-opt-debug : uninstall-bin-opt-debug uninstall-man

uninstall-bin-norm :
	$(rm) $(DESTDIR)$(bindir)/$(bin_norm_name)

uninstall-bin-debug :
	$(rm) $(DESTDIR)$(bindir)/$(bin_debug_name)

uninstall-bin-opt :
	$(rm) $(DESTDIR)$(bindir)/$(bin_opt_name)

uninstall-bin-opt-debug :
	$(DESTDIR)$(bindir)/$(bin_opt_debug_name)


clean :
	$(rm) $(bin_all_name) $(objects)

distclean : clean


dist :
