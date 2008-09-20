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

digram_hinting_values dhv[6] = { {19, 7, RANDSUB_HINT_DIGRAM_TH}, 
                                 {8, 13, RANDSUB_HINT_DIGRAM_IN}, 
                                 {8, 19, RANDSUB_HINT_DIGRAM_IT}, 
                                 {7, 4,  RANDSUB_HINT_DIGRAM}, 
                                 {4, 17, RANDSUB_HINT_DIGRAM}, 
                                 {0, 13, RANDSUB_HINT_DIGRAM} };

/* This function is _slow_! (because it does A LOT of jobs that a human should
 * such as guessing words etc.) THUS it should not be used in a bruteforce */
void crack_random_substitution(char *text, int text_size)
{
  digram digrams[26 * 26];
  hint_sort hint_sorts[26 * 26];
  int hinting[26][26];
  int i, h, j, k, l, m, n, p, q, r, s, t, u;
  int overall_match, improving;
  int best_freq, best_freq_num, best_freq_num2;
  int frequency_graph[26];
  int identity_frequency_graph[26];
  int translation_array[26];
  char ch;

  /* Setup hinting */
  for (i = 0; i < 26; i++) for (h = 0; h < 26; h++) hinting[i][h] = 0;

  /* Setup identity freq graph */ 
  create_identity_frequency_graph(identity_frequency_graph, text_size);

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
  for (i = 0; i < 5; i++)
  {
    m = h - i - 1; /* Location of the sorted digram */

    j =  digrams[m].digram_num % 26;
    h = (digrams[m].digram_num - j) / 26;
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
    if (identity_frequency_graph[best_freq_num] > text_size / 15)
    {
      hinting[i][best_freq_num] += k;
    }

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
 //   hinting[i][best_freq_num2] += l;
  }

  s = text_size / 26;       /* Used in hinting match increase */
  improving = 1;
  overall_match = 0;

  while (improving)
  {
    u = overall_match;
    overall_match = 0;

    /* Setup a match array */
    for (i = 0; i < 26; i++)  translation_array[i] = -1;

    /* Copy into our sorting struct */
    for (i = 0; i < 26; i++) for (j = 0; j < 26; j++)
    {
      m = (i * 26) + j;
      hint_sorts[ m ].source = i;
      hint_sorts[ m ].target = j;
      hint_sorts[ m ].score = hinting[i][j];
    }

    /* Sort */
    insertion_hinting_sort(hint_sorts, 26 * 26);

    /* Now extract the info */
    m = 26 * 26;
    for (i = (m - 1); i >= 0; i--)
    {
      /* Check if the target is already taken */
      p = 1;
      for (j = 0; j < 26; j++) 
      {
        if (translation_array[j] == hint_sorts[i].target)
        {
          p = 0;
        }
      }

      /* Check if the source is taken */
      if (translation_array[hint_sorts[i].source] != -1)
      {
        p = 0;
      }

      if (p && hint_sorts[i].score > RANDSUB_CUTOFF)
      {
        translation_array[hint_sorts[i].source] = hint_sorts[i].target;
      }
    }

    /* Debug.. */
    for (i = 0; i < text_size; i++)
    {
      ch = *(text + i);
      j = CHARNUM(ch);

      ch = (translation_array[j] == -1 ?
                          '.' : NUMCHAR(translation_array[j]));
      printf("%c", ch);
    }
    printf("\n\n");

    /* End Debug */

    l = text_size - MIN_WORD_SIZE;
    for (i = 0; i < l; i++)
    {
      fflush(stdout);

      h = min(text_size - i, WORD_BUF_SIZE);
      for (j = 0; j < h; j++)
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
            q = 0;

            for (p = 0; p < j; p++)
            {
              if (translation_array[ CHARNUM(*(text + i + p)) ] == 
                         CHARNUM(*(*(*(dict_length_pointer + j) + m) + p)))
              {
                q++;
              }
            }

            /* does it reach RANDSUB_LETTER_CUTOFF */
            if (q >= RANDSUB_MATCH_LETTER_CUTOFF)
            {
              for (r = 0; r < j; r++)
              {
                /* Source CHARNUM(*(text + i + r)) */
                /* Target CHARNUM(*(*(*(dict_length_pointer + j) + m) + r)) */

                n = CHARNUM(*(text + i + r));
                t = CHARNUM(*(*(*(dict_length_pointer + j) + m) + r));

                if (translation_array[q] != t)
                {
                  hinting[ q ][ t ] += (RANDSUB_MATCH_BASE * n) / j;
                }
              }
            }

            overall_match += q;
          }
        }
      }
    }

    if (overall_match > u)
    {
      improving = 1;
    }
    else
    {
      improving = 0;
    }
  }

  /* setup the match/letter sub array */
  for (i = 0; i < 26; i++)  translation_array[i] = -1;

  /* Copy into our sorting struct */
  for (i = 0; i < 26; i++) for (j = 0; j < 26; j++)
  {
    m = (i * 26) + j;
    hint_sorts[ m ].source = i;
    hint_sorts[ m ].target = j;
    hint_sorts[ m ].score = hinting[i][j];
  }

  /* Sort */
  insertion_hinting_sort(hint_sorts, 26 * 26);

  /* Now extract the info */
  m = 26 * 26;
  for (i = (m - 1); i >= 0; i--)
  {
    /* Check if the target is already taken */
    p = 1;
    for (j = 0; j < 26; j++)
    {  
      if (translation_array[j] == hint_sorts[i].target)
      {
        p = 0;
      }
    }

    /* Check if the source is taken */
    if (translation_array[hint_sorts[i].source] != -1)
    {  
      p = 0;
    }

    if (p && hint_sorts[i].score > RANDSUB_CUTOFF)
    {
      translation_array[hint_sorts[i].source] = hint_sorts[i].target;
    }
  }

  /* Do the translation - for any still unassigned make it an uppercase */
  for (i = 0; i < text_size; i++)
  {
    ch = *(text + i);
    j = CHARNUM(ch);

    *(text + i) = (translation_array[j] == -1 ? 
                        ALPHA_TOUPPER(ch) : NUMCHAR(translation_array[j]));
  }
}

