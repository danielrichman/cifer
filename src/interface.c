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

void print_freq(int freq[])
{
  int i;
 
  /* TODO: Some nice tabling thing please? sometimes the counts overflow */ 

  printf("Char | CHARNUM | Frequency\n\n");

  printf("C|");
  for (i = 0; i < 26; i++) printf("%2c|", NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%2i|", i);
  printf("\n");

  printf("F|");
  for (i = 0; i < 26; i++) printf("%2i|", freq[i]);
  printf("\n\n");

}

void print_char_table(void)
{
  int i;

  printf("Char | CHARNUM\n\n"); 

  printf("C|");
  for (i = 0; i < 26; i++) printf("%2c|", NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%2i|", i);
  printf("\n\n");
 
}
