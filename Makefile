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

opt_lvl = 2

dict_name = dict
dict_file = /usr/share/dict/british-english

bin_norm_name      = cifer
bin_debug_name     = cifer-debug
bin_opt_name       = cifer-opt
bin_opt_debug_name = cifer-opt-debug
bin_all_name       = $(bin_norm_name) $(bin_debug_name) $(bin_opt_name) \
                     $(bin_opt_debug_name)

cflags_debug = -pg
cflags_opt   = -O$(opt_lvl)
cflags_opt_debug = $(cflags_opt) -pg

objects = src/arg.o src/ciphers.o src/dictionary.o src/frequency_data.o \
          src/interface.o src/main.o src/utility.o src/vigenere.o \
          src/action.o src/affine.o src/keyword.o src/columnar_transposition.o \
          src/frequency_analysis.o src/urandom_access.o src/bacon.o src/io.o \
          src/polybius.o

cfiles = src/arg.c src/ciphers.c src/dictionary.c src/frequency_data.c \
         src/interface.c src/main.c src/utility.c src/vigenere.c \
         src/action.c src/affine.c src/keyword.c src/columnar_transposition.c \
         src/frequency_analysis.c src/urandom_access.c src/bacon.c src/io.c \
         src/polybius.c

common_headers = src/stdinc.h  src/macros.h  src/settings.h

$(bin_norm_name) : $(objects) $(dict_name)
	$(CC) $(CFLAGS) -o $(bin_norm_name) $(objects)

$(bin_debug_name) : $(dict_name) $(cfiles)
	$(CC) $(CFLAGS) $(cflags_debug) -o $(bin_debug_name) $(cfiles)

$(bin_opt_name) : $(dict_name) $(cfiles)
	$(CC) $(CFLAGS) $(cflags_opt) -o $(bin_opt_name) $(cfiles)

$(bin_opt_debug_name) : $(dict_name) $(cfiles)
	$(CC) $(CFLAGS) $(cflags_opt_debug) -o $(bin_opt_debug_name) $(cfiles)

src/arg.o                    : src/arg.h                    $(common-headers)
src/action.o                 : src/action.h                 $(common-headers)
src/affine.o                 : src/affine.h                 $(common-headers)
src/bacon.o                  : src/bacon.h                  $(common-headers)
src/ciphers.o                : src/ciphers.h                $(common-headers)
src/columnar_transposition.o : src/columnar_transposition.h $(common-headers)
src/dictionary.o             : src/dictionary.h             $(common-headers)
src/frequency_data.o         : src/frequency_data.h         $(common-headers)
src/frequency_analysis.o     : src/frequency_analysis.h     $(common-headers)
src/keyword.c                : src/keyword.h                $(common-headers)
src/interface.o              : src/interface.h              $(common-headers)
src/io.o                     : src/io.h                     $(common-headers)
src/main.o                   :                              $(common-headers)
src/polybius.o               : src/polybius.h               $(common-headers)
src/utility.o                : src/utility.h src/utility.i  $(common-headers) 
src/urandom_access.o         : src/urandom_access.h         $(common-headers)
src/vigenere.o               : src/vigenere.h               $(common-headers)

$(dict_name) :
	$(cat) $(dict_file) | sort > $(dict_name)

.PHONY : clean clean-objects

clean :
	$(rm) $(bin_all_name) $(dict_name) $(objects)

clean-objects :
	$(rm) $(objects)
