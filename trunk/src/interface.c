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
  int i, m;
  int width[26];

  printf("Char | CHARNUM | Frequency\n\n");

  m = 0;
  for (i = 0; i < 26; i++)
  {
    if (freq[i] < 10 && i < 10) width[i] = 1;
    else if (freq[i] < 100)     width[i] = 2;
    else if (freq[i] < 1000)    width[i] = 3;
    else                        width[i] = 4;

    if (width[i] > m)           m = width[i];
  }

  if (m <= 2)
  {
    /* Then we can afford to have them all at 2. It looks nicer */
    for (i = 0; i < 26; i++)    width[i] = 2;
  }

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], i);
  printf("\n");

  printf("F|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], freq[i]);
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

void print_trigrams(trigram input[], int size)
{
  int i;
  
  printf("Printing trigrams...\n");
  
  for (i = 0; i < size; i++)
  {
    printf("%c%c%c: %d\n", NUMCHAR(input[i].trigram_ch1), 
                           NUMCHAR(input[i].trigram_ch2), 
                           NUMCHAR(input[i].trigram_ch3),
                           input[i].trigram_value);
  }
}
