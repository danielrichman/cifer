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

digram_hinting_values dhv[5] = { {19, 7, RANDSUB_HINT_DIGRAM_TH}, 
                                 {7, 4, RANDSUB_HINT_DIGRAM}, 
                                 {8, 13, RANDSUB_HINT_DIGRAM}, 
                                 {4, 17, RANDSUB_HINT_DIGRAM}, 
                                 {0, 13, RANDSUB_HINT_DIGRAM} };

/* This function is _slow_! (because it does A LOT of jobs that a human should
 * such as guessing words etc.) THUS it should not be used in a bruteforce */
void crack_random_substitution(char *text, int text_size)
{
  digram digrams[26 * 26];
  int hinting[26][26], temp_hinting[26][26], best_cutoff_temp_hinting[26][26];
  int i, h, j, k, l, m, n, o, improving;
  int best_freq, best_freq_num, best_freq_num2;
  int best_cutoff, best_cutoff_score;
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

  k = text_size / RANDSUB_HINT_FREQ1;
  l = text_size / RANDSUB_HINT_FREQ2;

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
  best_cutoff = 0; best_cutoff_score = 0;
  n = RANDSUB_CUTOFF_MAX;   /* This could be a macro */
  o = RANDSUB_CUTOFF_INC;

  for (k = 0; k < n; k += o)
  {
    /* Zero temporary hinting var */
    /* Setup a match array */

    while (improving)
    {
      l = text_size - MIN_WORD_SIZE;
      for (i = 0; i < l; i++)
      {
        h = min(text_size - i, WORD_BUF_SIZE);
        for (j = 0; j < h; i++)
        {
          /* List of words of this length (j) are in dict_length_pointer[j] */
          /* dict_length_pointer[j] is of length dict_length_pointer_size[j] */
          /* See the possible TODO in dictionary.c with regard to cleaning up my
           * longhand array usage */

          if (*(dict_length_pointer_size + j) != 0)
          {
            for (m = 0; m < *(dict_length_pointer_size + j); m++)
            {
              /* see how well it macthes */
              /* does it reach RANDSUB_LETTER_CUTOFF */
              /* yes: add to temp_hinting taking into account
               * match/unkown letter ratio, RANDSUB_MATCH_RATIO_INCREASE 
               * and this letter's freqency. */
            }
          }
        }
      }
    }

    /* record the best */
    /* copy temp_hinting to best_cutoff_temp_hinting */
  }

  /* merge best_cutoff_temp_hinting & hinting via addition */
  /* setup the match/letter sub array */
  /* Do the translation */
}

