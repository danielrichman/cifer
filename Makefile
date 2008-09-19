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
          src/interface.o src/input.o src/main.o src/random_substitution.o \
          src/utility.o src/vigenere.o src/action.o

cfiles = src/arg.c src/ciphers.c src/dictionary.c src/frequency_data.c \
         src/interface.c src/input.c src/main.c src/random_substitution.c \
         src/utility.c src/vigenere.c src/action.c

stdinc = src/stdinc.h

cifer : $(objects) dict
	gcc -Wall -o cifer $(objects)

cifer-debug : dict
	gcc -Wall -g -o cifer-debug $(cfiles)

cifer-opt : dict
	gcc -Wall -O3 -o cifer-opt $(cfiles)

src/arg.o                 : src/arg.h                 $(stdinc)
src/action.o              : src/action.h              $(stdinc)
src/ciphers.o             : src/ciphers.h             $(stdinc)
src/dictionary.o          : src/dictionary.h          $(stdinc)
src/frequency_data.o      : src/frequency_data.h      $(stdinc)
src/interface.o           : src/interface.h           $(stdinc)
src/input.o               : src/input.h               $(stdinc)
src/main.o                :                           $(stdinc)
src/random_substitution.o : src/random_substitution.h $(stdinc)
src/utility.o             : src/utility.h             $(stdinc) 
src/vigenere.o            : src/vigenere.h            $(stdinc)

.PHONY : clean
clean :
	rm -rf cifer cifer-debug cifer-opt dict $(objects)

dict :
	cat /usr/share/dict/british-english | sort > dict
