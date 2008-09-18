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

#include "stdinc.h"

void crack_random_substitution(char *text, int text_size)
{
  digram digrams[26 * 26];
  int hinting[26 * 26];
  int i, h;

  /* To keep track of letters guessed and letters not, we use upper/lower */
  for (i = 0; i < text_size; i++) *(text + i) = ALPHA_TOUPPER(*(text + i));

  /* Start by finding the most common digram, TH */
  h = text_size - 1;
  for (i = 0; i < h; i++)
  {
    digrams[ (CHARNUM(*(text + i)) * 26) + CHARNUM(*(text + i + 1)) ].digram_value++;
  }

  h = 26 * 26;
  for (i = 0; i < h; i++) digrams[i].digram_num = i;

  insertion_digram_sort(digrams, h);

  /* Start with trigrams & common words. the, that, will, etc. */
  /* do the same for some bigrams. keep this minimal to avoid error */
  /* Fill in the remainder with the dictionary and frequency. */
  /* Perhaps some routine to fit partial-words and replace/fix a mistake 
   * using the dictionary and some bare skills */
}

