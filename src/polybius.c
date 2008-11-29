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

void polybius_encode(char *intext, int intext_size, char *outtext)
{
  int i, j, ch, pch, d1, d2;

  for (i = 0, j = 0; i < intext_size; i++, j += 2)
  {
    /* Grab the charnum and check its alpha ok */
    ch = CHARNUM(*(intext+ i));

    if (ch == -1)
    {
      *(outtext) = 0;
      return;
    }

    /* Convert to a polybius number */
    pch = polybius_grid_25_int[ch];

    /* Work out the digits */
    d2 = modp(pch, 10);
    d1 = pch - d2;

    /* Convert to ascii and save */
    *(outtext + j)     = NUMNUMCHAR(d1);
    *(outtext + j + 1) = NUMNUMCHAR(d2);
  }

  /* Null Termination */
  *(outtext + j) = 0;

  /* Results */
  printf("Polybius Encrypt:\n\n");
  printf("%s\n\n", outtext);
}

void polybius_decode(char *intext, int intext_size, char *outtext)
{
  int i, n, h, ptext_done;
  char ch1, ch2;
  
  /* Prepare */
  ptext_done = 0;
  *(outtext) = 0;

  /* Must be an even number of characters: action_ should check this */  
  if (modp(intext_size, 2) != 0)   return;

  /* Start looping! Take characters two at a time */
  for (i = 0; i < intext_size; i += 2)
  {
    ch1 = *(intext + i);
    ch2 = *(intext + i + 1);

    if (!NUMBER_CH(ch1) || !NUMBER_CH(ch2))
    {
      *(outtext) = 0;       /* Null the first char, effectivly empties it */
      return;
    }

    n = (NUMCHARNUM(ch1) * 10) + NUMCHARNUM(ch2);
    for (h = 0; h < 26; h++)
    {
      if (polybius_grid_25_int[h] == n)
      {
        /* Match */
        *(outtext + ptext_done) = NUMCHAR(h);
        ptext_done++;
        break;
      }
    }

    /* It got to the end!!! Match fail */
    if (h == 26)
    {
      *(outtext) = 0;
      return;
    }
  }

  /* Null termination */
  *(outtext + ptext_done) = 0;

  /* Results */
  printf("Polybius Decrypt:\n\n");
  printf("%s\n\n", outtext);
}

