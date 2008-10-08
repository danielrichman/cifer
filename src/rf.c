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

void rf_bf(char *text, int text_size, int minrails, int maxrails)
{
  int i, j, k, l, m, numloops, currails, currwl, currs, spaces1, spaces2;
  int *wl, *repeats, *repeats_rem, **chcount;
  int score, best_score, best_rails;
  char *ptext, *ptext_best;
  score_text_pro_state pro_state;

  /* Setup */
  maxrails++;   /* The code below takes maxrails as the maximum exclusive,
                   as its easier that way. Lets add one to include it */
  numloops = maxrails - minrails;
  if (numloops < 1)  return;
  if (minrails < 2)  return;

  wl          = malloc_good( sizeof(int)   * numloops );
  repeats     = malloc_good( sizeof(int)   * numloops );
  repeats_rem = malloc_good( sizeof(int)   * numloops );
  chcount     = malloc_good( sizeof(int *) * numloops );

  for (i = 0; i < numloops; i++)
    *(chcount + i) = malloc_good( sizeof(int) * maxrails );

  ptext       = malloc_good( text_size + 1 );
  ptext_best  = malloc_good( text_size + 1 );

  *(ptext      + text_size) = 0;
  *(ptext_best + text_size) = 0;

  best_rails = 0;
  best_score = -1;

  /* Makes -O2 happy =) */
  spaces1 = 0;
  spaces2 = 0;
  currs = 0;

  score_text_pro_start(text_size, &pro_state);
  /* End setup */

  /* Wavelength, for each rails */
  for (i = 0; i < numloops; i++)
    *(wl + i) = (minrails + i - 1) * 2;
  
  /* How many times the wave repeats, and the amount of the wave left over,
     for each rails */
  for (i = 0; i < numloops; i++)
  {
    *(repeats_rem + i) = modp(text_size, *(wl + i));
    *(repeats + i)     = (text_size - *(repeats_rem + i)) / *(wl + i);
  }

  /* chcount holds the count of chars for [currails - minrails][line] */
  for (i = 0; i < numloops; i++)
  {
    /* Precache */
    k = *(repeats + i);
    l = k * 2;

    /* The first and last line of each wave in rails has 1 ptext char */
    *(*(chcount + i))                = k;   /* First */
    *(*(chcount + i) + minrails + i) = k;   /* Last */

    /* The others have 2 ptext chars */
    for (j = 1; j < (maxrails - 1); j++)
      *(*(chcount + i) + j) = l;

    /* Handle that remainder! */
    k = 0;
    l = *(repeats_rem + i);
    for (j = 0; j < l; j++)
    {
      /* This increments k until it goes past i (rails) and then makes it
       * go back down; (k - i) = num gone past rail, i - (k - i) = going
       * back up the pattern; simplify to 2i - k */
      m = (k > (minrails + i) ? (((minrails + i) * 2) - k) : k);
      (*(*(chcount + i) + m))++;
    }
  }

  printf("Railfence Bruteforce: Starting %i loops, %i => %i\n",
                 numloops, minrails, maxrails - 1);

  /* Go pwn that cifaa */  
  for (currails = minrails; currails < maxrails; currails++)
  {
    /* Precache this from our array */
    currwl = *(wl + currails - minrails);

    /* Our chcount for this rail is chcount[currails - minrails][] */
    /* J is the end of the rail; I is the pointer in the ctext, k is 
     * rail num; L is the current target. */
    i = 0;
    j = 0;
    k = -1; /* This will be incremented to 0 straight away */
    l = 0;

    while (i < text_size)
    {
      /* This should happen at the end of each rail */
      if (j == i)
      {
        /* Work out when the next one ends */
        k++;
        j = *(*(chcount + currails - minrails) + k);

        /* Find out the two alternating spaces for this rail and setup 
         * the target start (l). */
        if (k == 0 || k == (currails - 1))
        {
          spaces1 = currwl;
          spaces2 = currwl;
        }
        else
        {
          spaces2 = k * 2;
          spaces1 = currwl - spaces2;
        }

        /* Flag the current spaces as (none); then set l to the init position */
        currs = -1;
        l = k;
      }

      /* Increment l; but only if it currs != -1 (none) */
      if      (currs == 1)   l += spaces1;
      else if (currs == 2)   l += spaces2;

      /* Place that ptext char where it belongs! */
      *(ptext + l) = *(text + i);

      /* Setup the next increment: if its spaces2 or none - set it to spaces1; 
       * else make it spaces2 */
      if (currs == 1)   currs = 2;
      else              currs = 1;

      i++;
    }

    /* Just in case */
    *(ptext + text_size) = 0;

    /* Now score that text */
    score = score_text_pro(ptext, &pro_state);

    if (score > best_score)
    {
      best_score = score;
      best_rails = currails;

      memcpy(ptext_best, ptext, text_size);
      *(ptext_best + text_size) = 0;  /* Be sure about \0 */
    }
  }

  /* Save it */
  memcpy(text, ptext_best, text_size);
  *(text + text_size) = 0;

  /* Print some info */
  currwl = *(wl + best_rails - minrails);
  printf("Railfence Bruteforce: Best Match %i:%i (%i)\n\n", 
                                     best_rails, currwl, best_score);

  /* Sorta like the loop above, but this prints an awesome pattern */
  for (i = 0; i < best_rails; i++)
  {
    if (i == 0 || i == (best_rails - 1))
    {
      spaces1 = currwl;
      spaces2 = currwl;
    }
    else
    {
      spaces2 = i * 2;
      spaces1 = currwl - spaces2;
    }

    l = i;
    currs = 1;

    /* And the rest */
    for (k = 0; k < 60; k++)
    {
      if (k == l)
      {
        if      (currs == 1) l += spaces1;
        else if (currs == 2) l += spaces2;

        if (currs == 1)    currs = 2;
        else               currs = 1;

        printf("X");
      }
      else
      {
        printf(".");
      }
    }

    printf(" %2i: %2i, %2i\n", i, spaces1, spaces2);
  }

  printf("\n");
  printf("ptext_best (saved to intext): \n\n");
  printf("%*s\n\n", text_size, text);

  /* Free up */
  for (i = 0; i < numloops; i++) free( *(chcount + i) );
  free(chcount);
  free(wl);
  free(repeats);
  free(repeats_rem);
  free(ptext);
  free(ptext_best);
}
