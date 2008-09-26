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

void bacon_dec(char *text, int text_size, int *new_size)
{
  int i, j, h, ptext_done;              /* Counters */
  char buf[5];

  /* Prepare */
  j = 0;                    /* Number of chars written into buf */
  ptext_done = 0;           /* Number of ptext chars written */
 
  /* Header & stats */
  printf("Cracking bacon cipher... text_size %d, (mod 5) %d\n", text_size,
                                                         modp(text_size, 5));
  
  for (i = 0; i < text_size; i++)
  {
    buf[j] = *(text + i);
    j++;

    /* We have one block filled, check it out */
    if (j == 5)
    {
      /* Check against each letter in the alphabet */
      for (h = 0; h < 26; h++)
      {
        if ((strncmp(buf, bacon_alphabet[h], 5)) == 0)
        { 
          /* We have a match! Don't worry about overlapping, we've 
           * buffered it so it should be fine */
          *(text + ptext_done) = NUMCHAR(h);
          ptext_done++;
          break;
        }
      }

      /* Reset the counter */
      j = 0;
    }
  }

  /* Update the null terminator and the size */
  *(text + ptext_done) = 0;
  *new_size = ptext_done;

  printf("Done.\n\n");
  printf("%*s\n\n", *new_size, text);
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
