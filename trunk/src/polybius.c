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

int polybius_grid_25_int[26] = { 11, 12, 13, 14,     15,
                                 21, 22, 23, 24, 24, 25,
                                 31, 32, 33, 34,     35,
                                 41, 42, 43, 44,     45,
                                 51, 52, 53, 54,     55 };

void polybius_dec(char *text, int text_size, int *new_size)
{
  int i, n, h, ptext_done;
  char ch1, ch2;
  
  printf("Doing polybius decrypt... text_size %d, mod 2 %d... \n",
         text_size, modp(text_size, 2));

  /* Prepare */
  ptext_done = 0;

  /* Must be an even number of characters. */  
  if (modp(text_size, 2))
  {
    printf("   failed, text_size %% 2 == %d (!= 0)!\n", modp(text_size, 2));
    return;
  }

  /* Start looping! Take characters two at a time */
  for (i = 0; i < text_size; i += 2)
  {
    ch1 = *(text + i);
    ch2 = *(text + i + 1);

    if (!NUMBER_CH(ch1) || !NUMBER_CH(ch2))
    {
      printf("  failed, non number char at %i or %i + 1 (%c %c)\n",
                 i, i, ch1, ch2);
      return;
    }   

    n = (NUMCHARNUM(ch1) * 10) + NUMCHARNUM(ch2);

    for (h = 0; h < 26; h++)
    {
      if (polybius_grid_25_int[h] == n)
      {
        /* Match */
        *(text + ptext_done) = NUMCHAR(h);
        ptext_done++;
        break;
      }
    }
  }

  /* Update the sizes and null terminators */
  *(text + ptext_done) = 0;
  *new_size = ptext_done;

  /* Results */
  printf("   succeeded:\n\n");
  printf("%*s\n\n", *new_size, text);
}
