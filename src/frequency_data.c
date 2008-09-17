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

double english_frequency[26] = { 0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 
                                 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 
                                 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 
                                 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 
                                 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 
                                 0.00074 };

void create_identity_frequency_graph(int frequency_graph[], int text_size)
{
  int i;

  for (i = 0; i < 26; i++)
  {
    frequency_graph[i] = english_frequency[i] * text_size;
  }
}

int *ch_freq(char *text, int input_size)
{
  printf("Doing letter frequency count...");
  
  int i;
  int *ch_freq;

  if ((ch_freq = malloc(26)) == NULL)
  {
    printf("\nMalloc failed! Exiting!\n");
    exit(1);
  }

  for (i = 0; i < 26; i++) *(ch_freq + i) = 0;

  for (i = 0; i < input_size; i++)
  {
    ch_freq[CHARNUM(*(text + i))] += 1;
  }

  printf("done\n");
  return ch_freq;
}
