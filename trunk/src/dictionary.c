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

/* The dictionary must be sorted, normal ubuntu dict has all captials then all
 * lowers, eg A-Z then a-z which Doesn't help =X 
 *    cat /usr/share/dict/british-english | sort > dict        */

#include "stdinc.h"

char *dict;
char **dict_pointer;
char **dict_pointer_end;

int score_text_dict_fast(char *text, int size)
{
  int i, jlen_buf, prefix, match_size, score;
  char *test_start, *test_end, *j;
  char ch1, ch2;

  match_size = 1;
  score = 0;

  /* If it gets to size - MIN_WORD_SIZE we won't find the last bit anyway 
   * because it's limited by MIN_WORD_SIZE */

  for (i = 0; i < (size - MIN_WORD_SIZE); i += match_size)
  {
    ch1 = *(text + i);
    ch2 = *(text + i + 1);

    prefix = (CHARNUM( ch1 ) * 26) + CHARNUM( ch2 );

    test_start = *(dict_pointer + prefix);
    test_end = *(dict_pointer_end + prefix);

    /* Find the smallest possibility */
    match_size = WORD_BUF_SIZE;

    for (j = test_start; j < test_end; j += jlen_buf + 1) /* Remember \0 */
    {
      /* In theory, the \0 terminators should take care of all size checks */
      jlen_buf = strlen(j);
      if (strcmp(j, text + i) == 0)    match_size = jlen_buf;
    }

    if (match_size != 1)
    {
      score += match_size;
    }
  }

  return score;
}

void load_dict(void)
{
  FILE *dictf;

  int i, j, filesize;
  int last_prefix, prefix;

  char ch;
  char *dict_insert, *dict_end;

  char buf[WORD_BUF_SIZE];
  int buf_size;

  int searching;

  dictf = fopen(DICTIONARY, "r");
  if (dictf == NULL)
  {
    printf("\n\n");
    perror("load_dict: fopen");
    printf("\n");

    exit(1);
  }

  /* Count filesize */
  filesize = 0;
  buf_size = 0;

  while (feof(dictf) == 0)
  {
    ch = getc(dictf);
    if (ALPHAL_CH(ch) || ALPHAH_CH(ch))
    {
      buf_size++;

      if (buf_size >= WORD_BUF_SIZE)
      {
        printf("Error: A Word busted WORD_BUF_SIZE %i\n", WORD_BUF_SIZE);
      }
    }
    else if (IS_NEWLINE(ch))
    {
      if (buf_size > MIN_WORD_SIZE)
      {
        filesize += buf_size + 1;
      }

      buf_size = 0;
    }
  }

  rewind(dictf);

  dict             = malloc( filesize + 1 );
  dict_pointer     = malloc( sizeof(char *) * ((26 * 26) + 1) );
  dict_pointer_end = malloc( sizeof(char *) * ((26 * 26) + 1) );

  if (dict == NULL || dict_pointer == NULL || dict_pointer_end == NULL)
  {
    /* Epic fail */
    printf("Malloc for dictionary failed. \n");
    exit(1);
  }

  dict_insert = dict;
  buf_size = 0;

  printf("Loading dictionary of %i bytes... ", filesize);
  fflush(stdout);

  /* Read it in */
  i = 0;

  while (feof(dictf) == 0)
  {
    ch = getc(dictf);

    if (ALPHAL_CH(ch) || ALPHAH_CH(ch))
    {
      buf[buf_size] = ch;
      buf_size++;
    }
    else if (IS_NEWLINE(ch))
    {
      if (buf_size > MIN_WORD_SIZE)
      {
        if ((i + buf_size) >= filesize)
        {
          printf("\n\n");
          printf("*dict overflow at %i (Attempted to add %i)\n", i, buf_size);
          printf("\n\n");

          exit(1);
        }

        memcpy(dict_insert, buf, buf_size);
        *(dict_insert + buf_size) = 0;

        dict_insert += buf_size + 1;
        i += buf_size + 1;
      }

      buf_size = 0;
    }
  }

  /* Close the file */
  fclose(dictf);

  printf("Done.\n");
  printf("Indexing the dictionary... ");
  fflush(stdout);

  /* Build up our quickjump pointaz */
  searching = 1;
  dict_insert = dict;
  last_prefix = -1;

  for (i = 0; i < (26 * 26); i++)
  {
    *(dict_pointer + i) = NULL;
    *(dict_pointer_end + i) = NULL;
  }

  /* Set the final one to be the very end; the final \0 to show filesize */
  dict_end = (dict + filesize);
  *(dict_pointer + (26 * 26))  = dict_end;

  while (searching)
  {
    j = strlen(dict_insert);

    if (j <= MIN_WORD_SIZE)
    {
      printf("Word length is %i (< %i), dict load fail.\n", j, MIN_WORD_SIZE);
      exit(1);
    }
    else
    {
      prefix = (CHARNUM( (char) *dict_insert ) * 26) +
                CHARNUM( (char) *(dict_insert + 1)     );
    }

    if (prefix != last_prefix)
    {
      /* Start the next prefix */
      *(dict_pointer + prefix) = dict_insert;

      /* Mark the end */
      if (last_prefix != -1)
      {
        dict_pointer_end[last_prefix] = dict_insert;
      }

      last_prefix = prefix;
    }

    dict_insert += j + 1;

    if (dict_insert >= dict_end)
    {
      searching = 0;
    }
  }

  printf("Done.\n");
  fflush(stdout);
}


