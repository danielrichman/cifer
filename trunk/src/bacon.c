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

void bacon_encode(char *intext, int intext_size, char *outtext)
{
  int i, j;
  char ch;

  printf("Encrypting using bacon cipher...\n");

  for (i = 0, j = 0; i < intext_size; i++)
  {
    ch = *(intext + i);

    if (ALPHA_CH(ch))
    {
      memcpy(outtext + j, bacon_alphabet[CHARNUM(ch)], 5);
      j += 5;
    }
    else
    {
      *(outtext + j) = ch;
      j++;
    }
  }

  *(outtext + j) = 0;
  printf("\n%s\n\n", outtext);
}

void bacon_decode(char *intext, int intext_size, char *outtext)
{
  int i, j, h, ptext_done;              /* Counters */
  char buf[5], ch;

  /* Prepare */
  j = 0;                    /* Number of chars written into buf */
  ptext_done = 0;           /* Number of ptext chars written */
 
  /* Header & stats */
  printf("Decoding bacon cipher... \n");

  for (i = 0; i < intext_size; i++)
  {
    ch = *(intext + i);

    if (CHARNUM(ch) == 0 || CHARNUM(ch) == 1)
    {
      buf[j] = ch;
      j++;
    }

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
          *(outtext + ptext_done) = NUMCHAR(h);
          ptext_done++;
          break;
        }
      }

      if (h == 26)
      {
        printf("Error: Invalid bacon '%s'\n", buf);
        return;
      }
 
      /* Reset the counter */
      j = 0;
    }
  }

  /* Update the null terminator and the size */
  *(outtext + ptext_done) = 0;
  printf("\n%s\n\n", outtext);
}

