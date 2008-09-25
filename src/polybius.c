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

/* TODO: fix bug where random crap is on the end of output */

#include "stdinc.h"

int polybius_grid_25_int[26] = { 11, 12, 13, 14,     15,
                                 21, 22, 23, 24, 24, 25,
                                 31, 32, 33, 34,     35,
                                 41, 42, 43, 44,     45,
                                 51, 52, 53, 54,     55 };

char *polybius_grid_25_char[26] = { "11", "12", "13", "14",       "15",
                                    "21", "22", "23", "24", "24", "25",
                                    "31", "32", "33", "34",       "35",
                                    "41", "42", "43", "44",       "45",
                                    "51", "52", "53", "54",       "55" };

int polybius_dec(char *text, int text_size)
{
  int i, h;
  char cur_set[2];
  int output_count = 0;
  int text_size_mod_2;
  
  printf("Doing polybius decrypt... text_size %d, mod 2 %d, ",
         text_size, (text_size_mod_2 = modp(text_size, 2)));
  
  if (text_size_mod_2 != 0)
  {
    printf("failed, text_size %% 2 == %d (!= 0)!\n", text_size_mod_2);
    return -1;
  }
  char output[text_size/2];
  
  for (i = 0; i < text_size; i += 2)
  {
    cur_set[0] = *(text + i);
    cur_set[1] = *(text + i + 1);
    
    for (h = 0; h < 26; h++)
    {
      if ((strncmp(cur_set, polybius_grid_25_char[h], 2)) == 0)
      {
        /* Match */
        output[output_count] = NUMCHAR(h);
        output_count++;
        break;
      }
    }
  }
  
  printf("\n\n%*s\n\n", output_count, output);
  return 0;
}
