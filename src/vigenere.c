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

void crack_vigenere(char *intext, int intext_size, char *outtext, 
                    int minb, int maxb)
{
  vigenere_column_ic *column_ic;
  int i, j, k, h;
  int *shift;
  double d;

  /* min/maxb are _inclusive_ */
  h = maxb - minb + 1;

  /* Prepare the loop */
  if (maxb > intext_size)
  {
    printf("Checks of length %i requested but ctext is only %i chars long.\n",
               maxb, intext_size);
    return;
  }

  if (h <= 0)
  {
    printf("vigenere: no column-lengths to try.\n");
    return;
  }

  /* Setup */
  column_ic = malloc_good( sizeof(column_ic) * h );
  printf("Attempting Vigenere Cipher Crack %i -> %i keylen\n", minb, maxb);

  /* Check all column lengths that we are supposed to */
  for (i = 0; i < h; i++)
  {
    column_ic[i].column_ic_diff = 0;
    k = minb + i;

    for (j = 0; j < k; j++)
    {
      column_ic[i].column_ic_diff += 
          diff( delta_ic(intext + j, intext_size - j, k), OPTIMAL_DELTA_IC);
    }

    column_ic[i].column_ic_diff = column_ic[i].column_ic_diff / k;
    column_ic[i].column_size = k;
  }

  /* Sort it */
  insertion_sort_vigenere_column_ic(column_ic, h);

  /* Prepare to decode... */
  h = column_ic[0].column_size;
  shift = malloc_good( sizeof(int) * h );

  /* The best match should now be in [0], so freq. analyse the columns */
  for (i = 0; i < h; i++)
  {
    *(shift + i) = frequency_analysis(intext + i, intext_size - i, h);
  }

  /* Do the magic! */
  caesar_cipher_dec(intext, intext_size, outtext, 
                    shift, column_ic[0].column_size);
  *(outtext + intext_size) = 0;

  /* Print out the keyword and deciphered plaintext */
  printf("Keyword: ");
  for (i = 0; i < h; i++) printf("%c", NUMCHAR(*(shift + i)));
  printf(" - ");
  for (i = 0; i < h; i++) printf("%i ", *(shift + i));
  printf("\n");

  d = delta_ic(outtext, intext_size, 1);
  printf("Overall Delta IC: %f\n\n", d);
  printf("%s\n\n", outtext);

  /* Free up... */
  free(shift);
}

