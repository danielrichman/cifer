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

objects = src/arg.o src/ciphers.o src/dictionary.o src/frequency_data.o \
          src/interface.o src/input.o src/main.o src/utility.o src/vigenere.o \
          src/action.o src/affine.o src/keyword.o  \
          src/frequency_analysis.o src/urandom_access.o

cfiles = src/arg.c src/ciphers.c src/dictionary.c src/frequency_data.c \
         src/interface.c src/input.c src/main.c src/utility.c src/vigenere.c \
         src/action.c src/affine.c src/keyword.c \
         src/frequency_analysis.c src/urandom_access.c

stdinc = src/stdinc.h  src/macros.h  src/settings.h

cifer : $(objects) dict
	gcc -Wall -o cifer $(objects)

cifer-debug : dict $(cfiles)
	gcc -Wall -g -o cifer-debug $(cfiles)

cifer-opt : dict $(cfiles)
	gcc -Wall -O3 -o cifer-opt $(cfiles)

src/arg.o                 : src/arg.h                   $(stdinc)
src/action.o              : src/action.h                $(stdinc)
src/affine.o              : src/affine.h                $(stdinc)
src/ciphers.o             : src/ciphers.h               $(stdinc)
src/dictionary.o          : src/dictionary.h            $(stdinc)
src/frequency_data.o      : src/frequency_data.h        $(stdinc)
src/frequency_analysis.o  : src/frequency_analysis.h    $(stdinc)
src/keyword.c             : src/keyword.h               $(stdinc)
src/interface.o           : src/interface.h             $(stdinc)
src/input.o               : src/input.h                 $(stdinc)
src/main.o                :                             $(stdinc)
src/utility.o             : src/utility.h src/utility.i $(stdinc) 
src/urandom_access.o      : src/urandom_access.h        $(stdinc)
src/vigenere.o            : src/vigenere.h              $(stdinc)

.PHONY : clean clean-objects
clean :
	rm -rf cifer cifer-debug cifer-opt dict $(objects)

clean-objects :
	rm -rf $(objects)

dict :
	cat /usr/share/dict/british-english | sort > dict
