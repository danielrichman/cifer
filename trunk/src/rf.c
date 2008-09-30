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

void rf_bf(char *intext, int intext_size, int maxrails)
{
}

int *rf_get_spaces(int rail)
{
  int i;
  int next;
  
  int *spaces = malloc(rail);
  memset(spaces, 0, rail);
  
  int fmiddle;
  
  if (rail % 2 == 0) /* If even */
  {
    fmiddle = rail/2;
    
    for (i = 0; i <= rail; i++)
    {
      if (i == (rail - 1) * 2)
      {
        *(spaces) = i; /* First */
        *(spaces + rail) = i; /* Last */
        
        next = rail - 2;
      }
      else
      if (i == fmiddle) *(spaces + fmiddle) = rail; /* First middle */
      else
      if (i == fmiddle + 1)
      {
        *(spaces + i) = rail; /* Last middle */
        next = rail + 2;
      }
      else
      if (i < fmiddle) /* We're in the first half, ie. decreasing values */
      {
        *(spaces + i) = next;
        next -= 2;
      }
      else
      if (i > fmiddle + 1) /* We're in the second half, ie. increasing values */
      {
        *(spaces + i) = next;
        next += 2;
      }
    }
  }
  else /* Is odd */
  {
    fmiddle = (rail + 1)/2;
    
    for (i = 0; i <= rail; i++)
    {
      if (i == (rail - 1) * 2)
      {
        *(spaces) = i; /* First */
        *(spaces + rail) = i; /* Last */

        next = rail - 2;
      }
      else
      if (i == fmiddle)
      {
        *(spaces + i) = rail; /* Middle */
        next = rail + 2;
      }
      else
      if (i < fmiddle) /* We're in the first half, ie. decreasing values */
      {
        *(spaces + i) = next;
        next -= 2;
      }
      else
      if (i > fmiddle) /* We're in the second half, ie. increasing values */
      {
        *(spaces + i) = next;
        next += 2;
      }
    }
  }
  
  return spaces;
}
