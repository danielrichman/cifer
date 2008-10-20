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

CC=gcc
CFLAGS=-Wall

rm = rm -rf
cat = cat
iconv = iconv
iconv_options = --from-code ISO_8859-1 --to-code ASCII//TRANSLIT 
sed = sed
sed_options = s/[^a-zA-Z]//g
sorter = sort
uniq = uniq
dd = dd
dd_options = conv=lcase

opt_lvl = 2

dict_name = dict
dict_file = /usr/share/dict/*-english

bin_norm_name      = cifer
bin_debug_name     = cifer-debug
bin_opt_name       = cifer-opt
bin_opt_debug_name = cifer-opt-debug
bin_all_name       = $(bin_norm_name) $(bin_debug_name) $(bin_opt_name) \
                     $(bin_opt_debug_name)

cflags_norm  = 
cflags_debug = -pg -g
cflags_opt   = -O$(opt_lvl)
cflags_opt_debug = $(cflags_opt) -pg -g

gprof_files = gmon.out

objects = src/affine.o \
	src/frequency_data.o \
	src/shell.o \
	src/bacon.o \
	src/interface.o \
	src/urandom_access.o \
	src/ciphers.o \
	src/io.o \
	src/utility.o \
	src/columnar_transposition.o \
	src/keyword.o \
	src/vigenere.o \
	src/command.o \
	src/main.o \
	src/vowel_mash.o \
	src/dictionary.o \
	src/polybius.o \
	src/frequency_analysis.o \
	src/rf.o

cfiles = src/affine.c \
	src/frequency_data.c \
	src/shell.c \
	src/bacon.c \
	src/interface.c \
	src/urandom_access.c   \
	src/ciphers.c \
	src/io.c \
	src/utility.c \
	src/columnar_transposition.c \
	src/keyword.c \
	src/vigenere.c \
	src/command.c \
	src/main.c \
        src/vowel_mash.c \
	src/dictionary.c \
	src/polybius.c \
	src/frequency_analysis.c \
	src/rf.c

headers = src/affine.h \
	src/frequency_data.h \
        src/settings.h \
	src/bacon.h \
	src/interface.h \
	src/shell.h \
	src/ciphers.h \
	src/io.h \
	src/stdinc.h \
	src/columnar_transposition.h \
	src/keyword.h \
	src/urandom_access.h \
	src/command.h \
	src/macros.h \
	src/utility.h \
	src/dictionary.h \
	src/polybius.h \
	src/vigenere.h \
	src/frequency_analysis.h \
	src/rf.h \
	src/vowel_mash.h

$(bin_norm_name) : $(objects) $(dict_name)
	$(CC) $(CFLAGS) $(cflags_norm)       -o $@ $(objects)

$(bin_debug_name) : $(dict_name) $(cfiles)
	$(CC) $(CFLAGS) $(cflags_debug)      -o $@ $(cfiles)

$(bin_opt_name) : $(dict_name) $(cfiles)
	$(CC) $(CFLAGS) $(cflags_opt)        -o $@ $(cfiles)

$(bin_opt_debug_name) : $(dict_name) $(cfiles)
	$(CC) $(CFLAGS) $(cflags_opt_debug)  -o $@ $(cfiles)

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

$(dict_name) :
	$(cat) $(dict_file) | \
        $(iconv) $(iconv_options) | \
	$(sed) $(sed_options) | \
        $(dd) $(dd_options) | \
	$(sorter) | $(uniq) \
        > $@

.PHONY : clean clean-objects

clean :
	$(rm) $(bin_all_name) $(dict_name) $(objects) $(gprof_files)

clean-objects :
	$(rm) $(objects)
