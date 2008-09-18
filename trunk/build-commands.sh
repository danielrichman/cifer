#!/bin/sh
[ ! -f dict ] || cat /usr/share/dict/british-english | sort > dict
gcc -o cifer src/*.c -Wall
