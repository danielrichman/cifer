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

/* This frequency counter & analyser is different in that it incorporates 
 * jumps; ie, columns */
int frequency_analysis(char *text, int text_size, int jump)
{
  int frequency_graph[26];
  int identity_frequency_graph[26];
  int i, j, temp_diff, best_diff, best_diff_shift;

  /* Prepare the variables */
  best_diff = -1;
  best_diff_shift = 0;
  j = 0;

  /* Zero the freq. graph and then scan the text */
  for (i = 0; i < 26; i++)  frequency_graph[i] = 0;

  /* J is used to temporarily store the total text/column size */
  for (i = 0; i < text_size; i += jump) 
  {
    frequency_graph[CHARNUM(*(text + i))] += 1;
    j++;
  }

  /* Setup the identity freq graphs */
  create_identity_frequency_graph(identity_frequency_graph, j);

  for (i = 0; i < 26; i++)
  {
    temp_diff = 0;

    /* Compare the new letters against the identity */
    for (j = 0; j < 26; j++)
    {
      temp_diff += diff(frequency_graph[((j + i) % 26)], 
                        identity_frequency_graph[j]);
    }

    /* And keep track of the best match */
    if (best_diff == -1 || temp_diff < best_diff)
    {
      best_diff = temp_diff;
      best_diff_shift = i;
    }
  }

  return best_diff_shift;
}

void caesar_cipher_enc(char *text, int text_size, int *shift, int shift_size)
{
  int i;

  for (i = 0; i < text_size; i++)
  {
    *(text + i) = NUMCHAR( ( CHARNUM( *(text + i) ) +
                             *(shift + (i % shift_size)) ) % 26);
  }
}

void caesar_cipher_dec(char *text, int text_size, int *shift, int shift_size)
{
  int i;

  for (i = 0; i < text_size; i++)
  {
    *(text + i) = NUMCHAR( ( CHARNUM( *(text + i) ) -
                             *(shift + (i % shift_size)) + 26 ) % 26);
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


