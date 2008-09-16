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

#define VIGNERE_MIN_KEYLEN 1
#define VIGNERE_MAX_KEYLEN 100
#define OPTIMAL_DELTA_IC 1.73

double english_frequency[26] = { 0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 
                                 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 
                                 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 
                                 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 
                                 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 
                                 0.00074 };

/* Attempts to crack text, modifies text at pointer and returns keyword */
void crack_vignere(char *text, int text_size)
{
  vignere_column_ic column_ic[VIGNERE_MAX_KEYLEN - VIGNERE_MIN_KEYLEN];
  int i, j, k, h;
  int *shift;
  double d;

  printf("Attempting Vignere Cipher Crack %i -> %i keylen\n", 
                VIGNERE_MIN_KEYLEN, VIGNERE_MAX_KEYLEN);

  /* Prepare the loop */
  h = min(VIGNERE_MAX_KEYLEN, text_size) - VIGNERE_MIN_KEYLEN;
  if (h <= 0)
  {
    printf("h <= 0; epic fail.\n");
    exit(1);
  }

  /* Check all column lengths that we are supposed to */
  for (i = 0; i < h; i++)
  {
    column_ic[i].column_ic_diff = 0;
    k = VIGNERE_MIN_KEYLEN + i;

    for (j = 0; j < k; j++)
    {
      column_ic[i].column_ic_diff += diff( delta_ic(text + j, text_size - j, k),
                                           OPTIMAL_DELTA_IC);
    }

    column_ic[i].column_ic_diff = column_ic[i].column_ic_diff / k;

    column_ic[i].column_size = k;
  }

  /* Sort it */
  insertion_columnic_sort(column_ic, VIGNERE_MAX_KEYLEN - VIGNERE_MIN_KEYLEN);

  /* Prepare to decode... */
  h = column_ic[0].column_size;
  shift = malloc( sizeof(int) * h );

  if (shift == NULL)
  {
    printf("shift = malloc( sizeof(int) * %i ) [%i bytes] failed.\n", 
                                          h, sizeof(int) * h);
    exit(1);
  }

  /* The best match should now be in [0], so freq. analyse the columns */
  for (i = 0; i < h; i++)
  {
    *(shift + i) = frequency_analysis(text + i, text_size - i, h);
  }

  /* Do the magic! */
  caesar_cipher_dec(text, text_size, shift, column_ic[0].column_size);

  /* Temporary Behaviour: Print out the keyword and deciphered plaintext */
  printf("Keyword: ");
  for (i = 0; i < h; i++) printf("%c", NUMCHAR(*(shift + i)));
  printf(" - ");
  for (i = 0; i < h; i++) printf("%i ", *(shift + i));
  printf("\n");

  d = delta_ic(text, text_size, 1);
  printf("Overall Delta IC: %f\n\n", d);
  printf("%*s\n\n", text_size, text);

  /* Free up... */
  free(shift);
}

void insertion_columnic_sort(vignere_column_ic a[], int asize)
{
  int i, j, k;
  vignere_column_ic d;

  k = asize - 1;

  for (i = 0; i < k; i++)
  {
    d = a[i];
    j = i - 1;

    while (j >= 0 && a[j].column_ic_diff > d.column_ic_diff)
    {
      a[j + 1] = a[j];
      j = j - 1;
    }

    a[j + 1] = d;
  }
}

int frequency_analysis(char *text, int text_size, int jump)
{
  int frequency_graph[26];
  int identity_frequency_graph[26];
  int i, j, temp_diff, best_diff, best_diff_shift;

  /* Prepare the variables */
  best_diff = -1;
  best_diff_shift = 0;

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

  /* For each shift... */
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
void create_identity_frequency_graph(int frequency_graph[], int text_size)
{
  int i;

  for (i = 0; i < 26; i++)
  {
    frequency_graph[i] = english_frequency[i] * text_size;
  }
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


