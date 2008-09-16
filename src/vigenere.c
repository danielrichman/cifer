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
