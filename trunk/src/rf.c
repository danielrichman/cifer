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
  int i, j, *spaces_temp;

  /* Setup reusable target space */
  spaces_temp = malloc( sizeof(int) * maxrails );

  for (i = minrails; i <= maxrails; i++)
  {
    rf_get_spaces(i, spaces_temp);
    printf("Rail %2i: %i", i, spaces_temp[0]);
    for (j = 1; j < i; j++) printf(", %i", spaces_temp[j]);
    printf("\n");
  }
}

void rf_get_spaces(int rail, int *target)
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
  printf("rf_get_spaces (%i): middle 1 and 2: %i %i\n", rail, middle1, middle2);

  for (i = 0; i < rail; i++)
  {
    if (i < middle1)  next -= 2;
    if (i > middle2)  next += 2;

    target[i] = next;
    printf("   -> debug, setting %i to new next %i\n", i, next);
  }
}

