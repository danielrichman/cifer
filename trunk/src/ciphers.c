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

void monoalph_substitute(char *text, int text_size, int *translation)
{
  int i;

  for (i = 0; i < text_size; i++)
  {
    *(text + i) = NUMCHAR(translation[CHARNUM(*(text + i))]);
  }
}

void caesar_cipher_enc(char *text, int text_size, int *shift, int shift_size)
{
  int i;

  for (i = 0; i < text_size; i++)
  {
    *(text + i) = NUMCHAR( modn( CHARNUM( *(text + i) ) +
                                 *(shift + modp(i, shift_size)), 26) );
  }
}

void caesar_cipher_dec(char *text, int text_size, int *shift, int shift_size)
{
  int i;

  for (i = 0; i < text_size; i++)
  {
    *(text + i) = NUMCHAR( modn( CHARNUM( *(text + i) ) -
                             *(shift + modp(i, shift_size)), 26) );
  }
}

double delta_ic (char *text, int text_size, int jump)
{
  int sfreq[26];
  int i, t;
  double j;

  j = 0; t = 0;

  /* Zero the char freq array; then count up the char freqs, then 
   * calculate (3rd for) the top half of the fraction, 4 - total the chars 
   * counted, finally, do the dividing from the bottom half */

  for (i = 0; i < 26; i++)                sfreq[i] = 0;
  for (i = 0; i < text_size; i += jump)   sfreq[CHARNUM(*(text + i))] += 1;
  for (i = 0; i < 26; i++)                j += sfreq[i] * (sfreq[i] - 1);
  for (i = 0; i < 26; i++)                t += sfreq[i];

  j = j / (t * (t - 1) / 26);

  return j;
}


