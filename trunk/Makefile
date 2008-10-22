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

ctags = ctags
ctags_files = src/*.c src/*.h src/*.i *.c *.h *.i
ctags_name = tags

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
	src/dictionary.h \
	src/frequency_analysis.h \
	src/frequency_data.h \
	src/interface.h \
	src/io.h \
	src/keyword.h \
	src/macros.h \
	src/polybius.h \
	src/rf.h \
	src/settings.h \
	src/shell.h \
	src/stdinc.h \
	src/utility.h \
	src/vigenere.h \
	src/vowel_mash.h \
	\
	src/utility.i \
	src/command_info.i \
	src/command.i

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

$(ctags_name):
	$(ctags) -f $@ $(ctags_files)

.PHONY : clean clean-objects

clean :
	$(rm) $(bin_all_name) $(dict_name) $(objects) $(gprof_files) \
	$(ctags_name)

clean-objects :
	$(rm) $(objects)
