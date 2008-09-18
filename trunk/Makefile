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
          src/interface.o src/main.o src/random_substitution.o src/utility.o \
          src/vigenere.o

cifer : $(objects)
	gcc -o cifer $(objects)

src/arg.o                 : src/arg.h
src/ciphers.o             : src/ciphers.h
src/dictionary.o          : src/dictionary.h
src/frequency_data.o      : src/frequency_data.h
src/interface.o           : src/interface.h
src/main.o                :
src/random_substitution.o : src/random_substitution.h
src/utility.o             : src/utility.h
src/vigenere.o            : src/vigenere.h

.PHONY : clean
clean :
	rm -rf cifer dict $(objects)

dict :
	cat /usr/share/dict/british-english | sort > dict
