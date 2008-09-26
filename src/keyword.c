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

/* TODO: Test this function's speed & effectivness */

void keyword_bruteforce(char *text, int text_size)
{
  char *j, *e, *best, *text_tmp, statusbuf[75];
  int len, score, best_score, temp_table[26];
  int p, lastp, l, lastl, lastplen;

  printf("keyword_bruteforce: Trying to find Keyword Cipher from the dict.\n");

  /* Dict will have been opened by action.c */
  e = *(dict_pointer + (26 * 26));  /* The end of the dictionary */

  /* Setup text_temp */
  text_tmp = malloc(text_size + 1);
  if (text_tmp == NULL)
  {
    printf("Malloc for text_tmp in keyword_brutefoce fail\n");
    return;
  }

  /* Null termination */
  *(text_tmp + text_size) = 0;

  /* Prepare */
  best = 0;
  best_score = 0;
  lastp = -1;
  lastl = -1;
  lastplen = 0;

  for (j = dict; j < e; j += len + 1)
  {
    len = strlen(j);

    l = CHARNUM(*j);
    p = (l * 26) + CHARNUM(*(j + 1));

    if (l != lastl)
    {
      if (lastl != -1)
      {
        keyword_table(best, strlen(best), temp_table);
        keyword_table_flip(temp_table);
        keyword_decode(text, text_tmp, text_size, temp_table);

        /* We have 80 - lastplen chars left. Spare 5 for ... \0, \n and ) */
        snprintf(statusbuf, 75 - lastplen, " (best so far: %i, %s: %s",
                  best_score, best,  text_tmp);

        /* Write out the now trimmed string (with the suffix blah...) \n */
        printf("%s...)", statusbuf);
      }

      printf("\n");
      fflush(stdout);
      lastplen = 0;
    }
    if (p != lastp)
    {
      printf("|");
      fflush(stdout);
      lastplen++;
    }

    lastl = l;
    lastp = p;

    keyword_table(j, len, temp_table);
    keyword_table_flip(temp_table);
    keyword_decode(text, text_tmp, text_size, temp_table);
    score = score_text_dict_fast(text_tmp, text_size);

    if (score > best_score)
    {
      best = j;
      best_score = score;
    }
  }

  /* Finish off the final bar */
  printf("\n\n");

  /* Free up */
  free(text_tmp);

  /* Now retrieve best */
  keyword_table(best, strlen(best), temp_table);
  keyword_table_flip(temp_table);
  keyword_decode(text, text, text_size, temp_table);
  keyword_table_flip(temp_table);
  keyword_print_info(text, text_size, best, strlen(best), temp_table);
}

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

int keyword_check(char *keyword, int key_size)
{
  int i;

  /* Check the keyword is ok! */
  for (i = 0; i < key_size; i++)
  {
    if (!ALPHAL_CH(*(keyword + i)) && !ALPHAH_CH(*(keyword + i)))
    {
      return 0;
    }
  }

  return 1;
}

/* This function will take a keyword and decode a keyword cipher
 * Note the intext and outtext, that's because we have to bruteforce
 * this routine and memcpy or a restoring-text for_loop would take
 * about 80% of the time up!!! (atleast on my pc) */
void keyword_decode(char *text, char *outtext, int text_size, int *table)
{
  int i;

  /* Translate... */
  for (i = 0; i < text_size; i++)
  {
    *(outtext + i) = NUMCHAR( table[CHARNUM(*(text + i))] );
  }
}

void keyword_table_copy(int *dest, int *source)
{
  int i;
  for (i = 0; i < 26; i++) dest[i] = source[i];
}

void keyword_decode_print(char *text, int text_size, 
                          char *keyword, int key_size)
{
  int table[26], otable[26];

  if (!keyword_check(keyword, key_size))
  {
    printf("Keyword Decode: Malformed keyword?\n\n");
    return;
  }

  keyword_table(keyword, key_size, table);
  keyword_table_copy(otable, table);
  keyword_table_flip(table);
  keyword_decode(text, text, text_size, table);
  keyword_print_info(text, text_size, keyword, key_size, otable);
}

void keyword_print_info(char *text, int text_size, 
                         char *keyword, int key_size, int *table)
{
  int m, i, width[26];

  /* The width should just end up as 2 everywhere */
  print_setup_width(width, &m);
  print_count_width(table, width, &m);
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
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(table[i]));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], table[i]);
  printf("\n\n");

  printf("%*s\n\n", text_size, text);
}
