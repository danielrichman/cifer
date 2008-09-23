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

char *bacon_alphabet[26] = { "AAAAA", "AAAAB", "AAABA", "AAABB", 
                             "AABAA", "AABAB", "AABBA", "AABBB", 
                             "ABAAA", "ABAAA", "ABAAB", "ABABA", 
                             "ABABB", "ABBAA", "ABBAB", "ABBBA", 
                             "ABBBB", "BAAAA", "BAAAB", "BAABA", 
                             "BAABB", "BAABB", "BABAA", "BABAB", 
                             "BABBA", "BABBB" };

int bacon_crack(char *intext, int intext_size)
{
  int i, h;                 /* Counters */
  int j = 0;                /* j is the current number of values in cur_set */
  int ptext_done = 0;       /* Number of ptext chars written */
  char cur_set[5];
  char ptext[intext_size/5];
  
  /* TODO: fail if intext_size % 5 != 0 */
  
  printf("Cracking bacon cipher... intext_size %d, (mod 5) %d\n", intext_size,
                                                         modp(intext_size, 5));
  
  for (i = 0; i < intext_size; i++)
  {
    for (j = 0; j < 5; j++)
    {
      cur_set[j] = *(intext + i);
    }
    
    j = 0;
    
    for (h = 0; h < 26; h++)
    {
      if ((strcmp(cur_set, bacon_alphabet[h])) == 0)
      { 
        /* We have a match! */
        ptext[ptext_done] = NUMCHAR(h);
        printf("%c", NUMCHAR(h));
      } else {
        /* Something bad happened; tell the user it's their fault xP */
      }
    }
  }
  
  return 0;
}

void bacon_upperlower_convert(char *text, int text_size)
{
  int i;
  for (i = 0; i < text_size; i++)
  {
    if (ALPHAH_CH(*(text + i))) *(text + i) = 'A';
    if (ALPHAL_CH(*(text + i))) *(text + i) = 'B';
  }
}
