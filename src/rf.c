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

void rf_bf(char *intext, int intext_size, int minrails, int maxrails)
{
  int i, j, currails; /* Counters */
  
  /* Wavelength, for each rails */
  int wl[maxrails];
  for (i = 0; i < maxrails; i++)
    wl[i] =  (i - 1) * 2;
  
  /* How many times the wave repeats, and the amount of the wave left over,
     for each rails */
  int repeats[maxrails];
  int repeats_rem[maxrails];
  for (i = 0; i < maxrails; i++)
  {
    repeats_rem[i] = modp(intext_size, wl[i]);
    repeats[i] = (intext_size - repeats_rem[i]) / wl[i];
  }
  
  /* chcount holds the count of chars for [num_of_rails][line] */
  /* Padded with -1 */
  int chcount[maxrails][maxrails]; /* We'll have space left over,
                                      stop crying and buy yourself some more
                                      RAM */
  for (i = 0; i < maxrails; i++) for (j = 0; j < maxrails; j++)
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
}

void rf_get_spaces(int rail, int target[][2])
{
  int i, next, middle1, middle2;

  if (modp(rail, 2) == 0)  /* Even */
  {
    middle1 = rail / 2;
    middle2 = middle1;
  }
  else
  {
    middle1 = (rail - 1) / 2;
    middle2 = (rail + 1) / 2;
  }

  next = rail * 2;

  for (i = 0; i < rail; i++)
  {
    if (i < middle1)  next -= 2;
    if (i > middle2)  next += 2;

    target[i][0] = next;
    
    if ((next - rail) < 0) /* Odd rails, middle line */
      target[i][1] = target[i][0];
    
    else if ((next - rail) == 0) /* Even rails, middle1/2 */
      target[i][1] = target[i][0] - 2;
    
    else if (next == rail) /* First or last */
      target[i][1] = target[i][0];
  }
}
