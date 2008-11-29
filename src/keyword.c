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

void keyword_bruteforce(char *intext, int intext_size, char *outtext)
{
  char *j, *e, *best, statusbuf[75];
  int len, score, best_score, temp_table[26];
  int i, p, lastp, l, lastl, lastplen;
  struct timeval seltime;
  fd_set stdset, stdset_temp;
  score_text_pro_state pro_state;

  /* Select() stuff */
  FD_ZERO(&stdset);
  FD_SET(0, &stdset); /* 0 is stdin */

  /* Zero these values for a non-blocking check */
  seltime.tv_sec  = 0;
  seltime.tv_usec = 0;

  printf("keyword_bruteforce: Trying to find Keyword Cipher from the dict.\n");

  /* Dict will have been opened by action.c */
  e = *(dict_pointer + (26 * 26));  /* The end of the dictionary */

  /* Setup Dictionary_pro */
  score_text_pro_start(intext_size, &pro_state);

  /* Null termination */
  *(outtext + intext_size) = 0;

  /* Prepare */
  best = 0;
  best_score = -1;
  lastp = -1;
  lastl = -1;
  lastplen = 0;
  
  printf("Press enter at any time to stop the brute force and use the last ");
  printf("best keyword.\n");

  for (j = dict; j < e; j += len + 1)
  {
    len = strlen(j);

    l = CHARNUM(*j);
    p = (l * 26) + CHARNUM(*(j + 1));

    if (l != lastl)
    {
      if (lastl != -1)
      {
        keyword_table_preflipped(best, strlen(best), temp_table);
        keyword_translate(intext, intext_size, outtext, temp_table);

        /* We have 80 - lastplen chars left. Spare 5 for ... \0, \n and ) */
        snprintf(statusbuf, 75 - lastplen, " (best so far: %i, %s: %s",
                  best_score, best,  outtext);

        /* Filter anything that will break our formatting */
        for (i = 0; i < (74 - lastplen); i++)
          if (XSPACE_CH(*(statusbuf + i)))
            *(statusbuf + i) = ' ';
        *(statusbuf + 75 - lastplen) = 0;

        /* Write out the now trimmed string (with the suffix blah...) \n */
        printf("%s...)", statusbuf);
        
        stdset_temp = stdset;
        select(1, &stdset_temp, NULL, NULL, &seltime);
        if (FD_ISSET(0, &stdset_temp))
          break;
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

    keyword_table_preflipped(j, len, temp_table);
    keyword_translate(intext, intext_size, outtext, temp_table);
    score = score_text_pro(outtext, &pro_state);

    if (score > best_score)
    {
      best = j;
      best_score = score;
    }
  }

  /* Finish off the final bar */
  printf("\n\n");

  /* Some Pro_check finish, stats and cleanup */
  score_text_pro_print_stats("keyword bruteforce", &pro_state);
  score_text_pro_cleanup(&pro_state);

  /* Now retrieve best */
  keyword_table_preflipped(best, strlen(best), temp_table);
  keyword_translate(intext, intext_size, outtext, temp_table);
  keyword_table_flip(temp_table);
  keyword_print_info(outtext, best, temp_table, "(bruteforce) decode");
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

void keyword_table_preflipped(char *keyword, int keyword_length, int *table)
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
      table[j] = t;
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
        table[j] = i;
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
    if (!ALPHA_CH(*(keyword + i)))
    {
      return -1;
    }
  }

  return 0;
}

/* This function will take a keyword and decode a keyword cipher
 * Note the intext and outtext, that's because we have to bruteforce
 * this routine and memcpy or a restoring-text for_loop would take
 * about 80% of the time up!!! (atleast on my pc) */
void keyword_translate(char *intext, int intext_size, char *outtext, int *table)
{
  int i, c;
  char o;

  /* Translate... */
  for (i = 0; i < intext_size; i++)
  {
    o = *(intext + i);
    c = CHARNUM(o);

    if (c != -1) o = NUMCHAR( table[c] );
    *(outtext + i) = o;
  }
}

void keyword_table_copy(int *dest, int *source)
{
  int i;
  for (i = 0; i < 26; i++) dest[i] = source[i];
}

void keyword_single(char *intext, int intext_size, char *outtext,
                    char *keyword, int key_size, int flip)
{
  int table[26], otable[26];

  keyword_table(keyword, key_size, table);
  keyword_table_copy(otable, table);
  if (flip) keyword_table_flip(table);
  keyword_translate(intext, intext_size, outtext, table);
  *(outtext + intext_size) = 0;
  keyword_print_info(outtext, keyword, otable,
                              (flip ? "decode" : "encode"));
}

void keyword_print_keyinfo(int *table)
{
  int i;

  printf("Plaintext Char | Ciphertext Char\n");
  printf("P|");
  for (i = 0; i < 26; i++) printf("%2c|", NUMCHAR(i));
  printf("\n");

  printf("C|");
  for (i = 0; i < 26; i++) printf("%2c|", NUMCHAR(table[i]));
  printf("\n\n");
}

void keyword_print_info(char *text, char *keyword,
                        int *table, char *dirstring)
{
  printf("Keyword Cipher %s using keyword %s: \n\n", dirstring, keyword);
  keyword_print_keyinfo(table);
  printf("%s\n\n", text);
}
