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
#define RF_WL(rail) ((rail - 1) * 2)

void rf_bf(char *intext, int intext_size, int minrails, int maxrails)
{
  int i, j, k, numloops, currails; /* Counters */
  
  /* Wavelength, for each rails */
  int wl[maxrails];
  for (i = 0; i < maxrails; i++)
    wl[i] = RF_WL(i);
  
  /* How many times the wave repeats, and the amount of the wave left over,
     for each rails */
  int repeats[maxrails];
  int repeats_rem[maxrails];
  for (i = 0; i < maxrails; i++)
  {
    repeats_rem[i] = modp(intext_size, wl[i]);
    repeats[i] = (intext_size - repeats_rem[i]) / wl[i];
  }

  numloops = maxrails - minrails;
  if (numloops < 1) panic();

  /* chcount holds the count of chars for [num_of_rails][line] */
  /* Padded with -1 */
  int chcount[numloops][maxrails]; /* We'll have space left over,
                                      stop crying and buy yourself some more
                                      RAM */
  for (i = 0; i < numloops; i++) for (j = 0; j < maxrails; j++)
    chcount[i][j] = -1;
  
  /* The first and last line of each wave in rails has 1 ptext char */
  for (i = 0; i < maxrails; i++)
  {
    chcount[i][0] = repeats[i]; /* First */
    chcount[i][i] = repeats[i]; /* Last */
  }
  
  /* All the others have 2 ptext chars */
  for (i = 0; i < maxrails; i++) for (j = 1; j < (maxrails - 1); j++)
    chcount[i][j] = repeats[i] * 2;

  for (currails = minrails; currails < maxrails; currails++)
  {
    /* Our chcount for this rail is chcount[currails - minrails][] */
    /* DO some pwn */

    /* J is the end of the rail; I is the pointer in the ctext, k is 
     * rail num */
    i = 0; j = 0; k = 0;

    while (i < text_size)
    {
      /* Find out the two alternating spaces for this rail! */

      /* This should happen at the start and end of each rail */
      if (j == i)
      {
        /* end of the rail, work out when the next one ends */
      }

      // work out the target (DR Shotguns later)
      // move text[i] to outtext[target]
    }     
  }
}