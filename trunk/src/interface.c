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

char *intext;
int intext_size;

/* TODO: Be rid of this function */
void cc_setup(int argc, char **argv)
{
  load_dict();
}

void print_freq(int *freq, int size)
{
  int i;
  int j = 0;
  
  printf("Printing letter frequency count... \n");
  
  for (i = 0; i < size; i++)
  {
    printf("%c:%d ", NUMCHAR(i), *(freq + i));
    j++;
    if (j == 5)
    {
      printf("\n");
      j = 0;
    }
  }
  printf("\n");
}

void print_char_table(void)
{
  int i;
  int j = 0;
  
  printf("Printing char -> number table... \n");
  
  for (i = 0; i < 26; i++)
  {
    printf("%c:%d ", NUMCHAR(i), i + 1);
    j++;
    if (j == 5)
    {
      printf("\n");
      j = 0;
    }
  }
  printf("\n");
}
