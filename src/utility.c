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

void insertion_columnic_sort(vigenere_column_ic a[], int asize)
{
  int i, j, k;
  vigenere_column_ic d;

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

