/*
    Cifer: Automating classical cipher cracking in C
    Copyright (C) 2008  Daniel Richman & Simrun Basuita

    Cifer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cifer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cifer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STDINC_CIPHERCRACK
#define STDINC_CIPHERCRACK 1

#include <stdio.h>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "vigenere.h"

#define VIGNERE_MIN_KEYLEN 1
#define VIGNERE_MAX_KEYLEN 100

#define OPTIMAL_DELTA_IC 1.73

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))
#define diff(X, Y) ((X) > (Y) ? ((X) - (Y)) : ((Y) - (X)))

/* 48 - 57 (numbers) 65 - 90 (letters uppercase) 97 - 122 (letters lowercase) */
/* Printable characters (eg. "£$%^&*, numbers, space) 32 - 128) */
#define ASCII_CH(ch)           (ch >= 32 && ch <= 128)
#define ALPHAL_CH(ch)          (ch >= 97 && ch <= 122)
#define ALPHAH_CH(ch)          (ch >= 65 && ch <= 90)
#define NUMBER_CH(ch)          (ch >= 48 && ch <= 57)
#define ALPHANUMERIC_CH(ch)    (ALPHAL_CH(ch) || ALPHAH_CH(ch) || NUMBER_CH(ch))
#define SPACE_CH(ch)           (ch == 32)

#define ALPHA_TOUPPER(ch)      (ALPHAL_CH(ch) ? ch - 32 : ch)
#define ALPHA_TOLOWER(ch)      (ALPHAH_CH(ch) ? ch + 32 : ch)

#define IS_NEWLINE(ch)         (ch == 10)
#define CHARNUM(ch)            (ALPHAL_CH(ch) || ALPHAH_CH(ch) ? \
                                   ALPHA_TOLOWER(ch) - 97 : -1)
#define NUMCHAR(i)             (i + 97)

typedef struct {
  int column_size;
  double column_ic_diff;
} vignere_column_ic;

/* Function Declarations */
void crack_vignere(char *text, int text_size);
void insertion_columnic_sort(vignere_column_ic a[], int asize);
int frequency_analysis(char *text, int text_size, int jump);
void create_identity_frequency_graph(int frequency_graph[], int text_size);
void caesar_cipher_enc(char *text, int text_size, int *shift, int shift_size);
void caesar_cipher_dec(char *text, int text_size, int *shift, int shift_size);
double delta_ic (char *text, int text_size, int jump);
void cc_setup(int argc, char **argv);

/* Super-Global Variable declarations */
extern char *intext;
extern int intext_size;

#endif

