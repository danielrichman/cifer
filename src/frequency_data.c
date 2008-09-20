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

int *ch_freq_ar;

double english_frequency[26] = { 0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 
                                 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 
                                 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 
                                 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 
                                 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 
                                 0.00074 };

void create_identity_frequency_graph(int frequency_graph[], int text_size)
{
  int i;

  for (i = 0; i < 26; i++)
  {
    frequency_graph[i] = english_frequency[i] * text_size;
  }
}

/* Counts letters 0 -> 26 into the tgt array as [key == letter] => count */
/* Does not support jump/column implementation like frequency_analysis does */
void count_freq(char *text, int input_size, int tgt[])
{
  printf("Doing letter frequency count... ");
  
  int i;

  for (i = 0; i < 26; i++)          tgt[i] = 0;
  for (i = 0; i < input_size; i++)  tgt[ CHARNUM(*(text + i)) ]++;
}

/* Counts digrams, sorts them, and loads the top `tgt_size` into tgt[] */
void count_digrams(char *text, int input_size, digram tgt[], int tgt_size)
{
  int i, h, j, k, l;
  char ch1, ch2;
  digram digrams[26 * 26];

  /* Setup */
  h = 0;
  for (i = 0; i < 26; i++) for (j = 0; j < 26; j++) 
  {
    digrams[h].digram_ch1 = i;
    digrams[h].digram_ch2 = j;
    digrams[h].digram_value = 0;
    h++;
  }

  /* Count */
  h = input_size - 1;
  for (i = 0; i < h; i++)
  {
    ch1 = *(text + i);
    ch2 = *(text + i + 1);

    j = CHARNUM(ch1);
    k = CHARNUM(ch2);
    l = (j * 26) + k;

    digrams[l].digram_value ++;
  }

  /* Sort */
  insertion_digram_sort(digrams, 26 * 26);

  /* Grab the top few */
  for (i = 0; i < tgt_size; i++)
  {
    h = (26 * 26) - 1 - i;
    tgt[i] = digrams[h];
  }
}

void count_trigrams(char *text, int input_size, trigram tgt[], int tgt_size)
{
  int i, h, j, k, l, m;
  char ch1, ch2, ch3;
  trigram trigrams[26 * 26 * 26];   /* You know something's wrong when one stack
                                     * frame is 70KB. =P */

  /* Setup */
  h = 0;
  for (i = 0; i < 26; i++) for (j = 0; j < 26; j++) for (k = 0; k < 26; k++)
  {
    trigrams[h].trigram_ch1 = i;
    trigrams[h].trigram_ch2 = j;
    trigrams[h].trigram_ch3 = k;
    trigrams[h].trigram_value = 0;
    h++;
  }

  /* Count */
  h = input_size - 2;
  for (i = 0; i < h; i++)
  {
    ch1 = *(text + i);
    ch2 = *(text + i + 1);
    ch3 = *(text + i + 2);

    j = CHARNUM(ch1);
    k = CHARNUM(ch2);
    l = CHARNUM(ch3);
    m = (j * (26 * 26)) + (k * 26) + l;

    trigrams[m].trigram_value ++;
  }

  /* Sort */
  insertion_trigram_sort(trigrams, 26 * 26 * 26);

  /* Grab the top few */
  for (i = 0; i < tgt_size; i++)
  {
    h = (26 * 26 * 26) - 1 - i;
    tgt[i] = trigrams[h];
  }
}

