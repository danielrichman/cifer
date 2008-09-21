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

/* This function fills out an int array[26] from the keyword */
/* in this table, the KEY is the plaintext. */
void keyword_table(char *keyword, int keyword_length, int *table)
{
  int i, j, t;
  int used_letters[26];

  /* Setup. */
  for (i = 0; i < 26; i++) used_letters[i] = 0;
  t = 0;

  /* Copy the keyword in, removing duplicate letters */
  for (i = 0; i < keyword_length; i++)
  {
    j = CHARNUM(*(keyword + i));
    if (!used_letters[j])
    {
      table[t] = j;
      t++;

      used_letters[j] = 1;
    }
  }

  /* Fill the remainder of the table. */
  for (i = t; i < 26; i++)
  {
    /* Find an unused letter */
    for (j = 0; j < 26; j++)
    {
      if (!used_letters[j])
      {
        used_letters[j] = 1;
        table[i] = j;
        break;   /* Break the J loop only. */
      }
    }
  }
}

/* This takes a array[plaintext]  => ciphertext and makes it into
 *              array[ciphertext] => plaintext */
void keyword_table_flip(int *table)
{
  int i;
  int temp[26];

  /* 1) copy table into temp */
  for (i = 0; i < 26; i++) temp[i] = table[i];

  /* 2) now flip keys and values... */
  for (i = 0; i < 26; i++) table[temp[i]] = i;
}

/* This function will take a keyword and decode a keyword cipher */
void keyword_decode(char *text, int text_size, char *keyword, int key_size)
{
  int i, m;
  int width[26], otable[26], table[26];

  /* Check the keyword is ok! */
  for (i = 0; i < key_size; i++)
  {
    if (!ALPHAL_CH(*(keyword + i)) && !ALPHAH_CH(*(keyword + i)))
    {
      printf("Keyword Decode: Malformed keyword?\n\n");
      return;
    }
  }

  /* Setup the table */
  keyword_table(keyword, key_size, table);
  for (i = 0; i < 26; i++) otable[i] = table[i];
  keyword_table_flip(table);

  /* Translate... */
  for (i = 0; i < text_size; i++)
  {
    *(text + i) = NUMCHAR( table[CHARNUM(*(text + i))] );
  }

  /* The width should just end up as 2 everywhere */
  print_setup_width(width, &m);
  print_count_width(otable, width, &m);
  print_finalise_width(width, &m);

  printf("Keyword Cipher Decode using keyword %s: \n\n", keyword);

  printf("Plaintext Char | PCharNum | Ciphertext Char | CCharNum\n");

  printf("P|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], i);
  printf("\n");

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(otable[i]));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], otable[i]);
  printf("\n\n");

  printf("%*s\n\n", text_size, text);
}
