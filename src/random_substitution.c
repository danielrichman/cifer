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

typedef struct {
  int digram_ch1;
  int digram_ch2;
  int digram_hint;
} digram_hinting_values;

digram_hinting_values dhv[5] = { {19, 7, 2}, {7, 4, 10}, {8, 13, 10}, 
                                 {4, 17, 10}, {0, 13, 10} };

/* This function is _slow_! (because it does A LOT of jobs that a human should
 * such as guessing words etc.) THUS it should not be used in a bruteforce */
void crack_random_substitution(char *text, int text_size)
{
  digram digrams[26 * 26];
  int hinting[26][26];
  int i, h, j, k, l, best_freq, best_freq_num, best_freq_num2;
  int frequency_graph[26];
  int identity_frequency_graph[26];

  /* Setup hinting */
  for (i = 0; i < 26; i++) for (h = 0; h < 26; h++) hinting[i][h] = 0;

  /* Setup identity freq graph */ 
  create_identity_frequency_graph(identity_frequency_graph, text_size);

  /* Note: REVISE: this isn't actually used. removed incase of future possible
   * use. */
  /* To keep track of letters guessed and letters not, we use upper/lower */
  /* for (i = 0; i < text_size; i++) *(text + i) = ALPHA_TOUPPER(*(text + i));*/

  /* Start by finding the most common digram, TH */
  h = text_size - 1;
  for (i = 0; i < h; i++)
  {
    digrams[ (CHARNUM(*(text + i)) * 26) + 
              CHARNUM(*(text + i + 1))      ].digram_value++;
  }

  h = 26 * 26;
  for (i = 0; i < h; i++) digrams[i].digram_num = i;

  insertion_digram_sort(digrams, h);

  /* Now heavily hint TH as the first, he, in, er, an for the next four */
  for (i = 0; i < 5; i++)  /* Sizeof DHV = 5 */
  {
    j =  digrams[i].digram_num % 26;
    h = (digrams[i].digram_num - j) / 26;
    k = text_size / dhv[i].digram_hint;

    hinting[h][dhv[i].digram_ch1] += k;
    hinting[j][dhv[i].digram_ch2] += k;
  }

  /* Setup the frequency counts */
  for (i = 0; i < 26; i++) frequency_graph[i] = 0;
  for (i = 0; i < text_size; i++) frequency_graph[CHARNUM(*(text + i))]++;

  k = text_size / 7;
  l = text_size / 14;

  /* Find the best frequency match and then the second best */
  for (i = 0; i < 26; i++)
  {
    /* For frequency_graph[i] find the closest match to the value */
    best_freq = -1;

    for (j = 0; j < 26; j++)
    {
      h = diff(frequency_graph[i], identity_frequency_graph[j]);

      if (best_freq == -1 || h < best_freq)
      {
        best_freq = h;
        best_freq_num = j;
      }
    }

    /* Hint the best match += text_size / 7 (precached into variable k) */
    hinting[i][best_freq_num] += k;

    /* Now find the second closest match */
    best_freq = -1;

    for (j = 0; j < 26; j++)
    {
      if (j != best_freq_num)
      {
        h = diff(frequency_graph[i], identity_frequency_graph[j]);

        if (best_freq == -1 || h < best_freq)
        {
          best_freq = h;
          best_freq_num2 = j;
        }
      }
    }

    /* And hint the second match text_size / 14 (precached into variable l) */
    hinting[i][best_freq_num2] += l;
  }

  /* start at a very low cutoff (k is now the hinting cutoff) */
  k = 0;

  /* Loop through. try to match values to the dictionary. matched words have
   * += 1 hinting to each of their letters. Overall match is tracked in j 
   * if its a general epic failure of a match, the cutoff value is raised. 
   * This roots out bad decisions. += 1 hinting to word matches letters is 
   * cached into a temporary hinting variable. If the whole shebang is 
   * successfull, then they are committed. Words that match for all but
   * one letter have that letter deducted and the correct one added
   * TODO: Implement some system that means that if the chance is higher 
   * (more letters in that word match than not) then the hinting bonus
   * is greater */

  /* Utilise hinting & the cutoff to do the translation. */
}

