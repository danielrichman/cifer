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

#define VIGENERE_MIN_KEYLEN 1
#define VIGENERE_MAX_KEYLEN 12                /* Fixed */
// #define VIGENERE_MAX_KEYLEN text_size      /* Size of ctext */
/* Todo: Make the max keylen dynamically increase using the auto-detect stuff */

#define DICTIONARY "./dict"
#define WORD_BUF_SIZE  30      /* Slightly more than `cat dict | wc -L` */
#define MIN_WORD_SIZE  2

#define OPTIMAL_DELTA_IC 1.73

/* TODO: these figures need testing & revising */
#define RANDSUB_CUTOFF          0

#define RANDSUB_HINT_DIGRAM_TH  2    /* as a text_size / x */
#define RANDSUB_HINT_DIGRAM_IT  5    /* as a text_size / x */
#define RANDSUB_HINT_DIGRAM_IN  9    /* as a text_size / x */
#define RANDSUB_HINT_DIGRAM     10

#define RANDSUB_HINT_FREQ1      7
#define RANDSUB_HINT_FREQ2      14

#define RANDSUB_MATCH_LETTER_CUTOFF  2

#define RANDSUB_MATCH_BASE      2

