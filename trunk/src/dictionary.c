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

void score_text_pro_start(int text_size, score_text_pro_state *state)
{
  if (text_size < 2)
  {
    printf("score_text_pro will not work with text_size < 2; failing.\n");
    state->text_size = 0;
    return;
  }

  /* Pre malloc all the space needed */
  state->text_size = text_size;
  state->frequency_graph_tolerance = 200;
  state->frequency_graph          = malloc( sizeof(int) * 26 );
  state->identity_frequency_graph = malloc( sizeof(int) * 26 );
  state->digrams_temp  = malloc( sizeof(digram)  * 26 * 26 );
  state->trigrams_temp = malloc( sizeof(trigram) * 26 * 26 * 26 );

  if (state->frequency_graph == NULL ||
      state->identity_frequency_graph == NULL ||
      state->digrams_temp == NULL ||
      state->trigrams_temp == NULL)

  {
    printf("score_text_pro_start: failed to malloc enough space to *state\n");
    exit(1);
  }

  /* Setup id. frequency graph */
  create_identity_frequency_graph(state->identity_frequency_graph, text_size);
}

void score_text_pro_cleanup(score_text_pro_state *state)
{
  free(state->digrams_temp);
  free(state->trigrams_temp);
  free(state->identity_frequency_graph);
}

int score_text_pro(char *text, score_text_pro_state *state)
{
  int i, j, k, h, l, fg_diff;
  trigram temp_the;
  /* This routine is made to quickly discard garbage but generate a better
   * score for real matches */

  if (state->text_size == 0) return 0;

  /* Reset the variables */
  fg_diff = 0;
  for (i = 0; i < 26; i++)   state->frequency_graph[i] = 0;

  /* This will have been sorted, so it all must be reset */
  h = 0;
  for (i = 0; i < 26; i++) for (j = 0; j < 26; j++)
  {
    state->digrams_temp[h].digram_ch1 = i;
    state->digrams_temp[h].digram_ch2 = j;
    state->digrams_temp[h].digram_value = 0;
    h++;
  }

  /* Same */
  h = 0;
  for (i = 0; i < 26; i++) for (j = 0; j < 26; j++) for (k = 0; k < 26; k++)
  {
    state->trigrams_temp[h].trigram_ch1 = i;
    state->trigrams_temp[h].trigram_ch2 = j;
    state->trigrams_temp[h].trigram_ch3 = k;
    state->trigrams_temp[h].trigram_value = 0;
    h++;
  }

  /* This loop will search through, matching all single letters above
   * [0] and [1] (because trigams say so) and all digrams except [1]; 
   * which must be counted afterwards */

  /* Combine the Frequency analysis, digram + trigram loops into one */
  l = state->text_size - 2;  /* For Trigrams */
  for (i = 0; i < l; i++)
  {
    j =  CHARNUM(*(text + i + 2));
    h = (CHARNUM(*(text + i + 1)) * 26 ) + j;
    k = (CHARNUM(*(text + i))     * 676) + h;
    state->trigrams_temp[k].trigram_value ++;
    state->digrams_temp[h].digram_value   ++;
    state->frequency_graph[j]             ++;
  }

  i = CHARNUM(*text);
  j = CHARNUM(*(text + 1));
  h = (i * 26) + j;

  state->digrams_temp[h].digram_value ++;
  state->frequency_graph[i] ++;
  state->frequency_graph[j] ++;

  /* Generate some sort of "diff" for the frequency graph */
  for (i = 0; i < 26; i++)
  {
    fg_diff += diff(state->frequency_graph[i], 
                    state->identity_frequency_graph[i]);

    printf("%c: %3i, %3i, %3i\n", NUMCHAR(i), state->frequency_graph[i],
                                       state->identity_frequency_graph[i], 
                    diff(state->frequency_graph[i],
                         state->identity_frequency_graph[i]));
  }

  printf("fg_diff: %i\n", fg_diff);

  /* Does it meet the cutoff ? */
  if (fg_diff > state->frequency_graph_tolerance)
  {
    /* Missed the cutoff; return a score < 100 */
    return max(100 - (fg_diff - state->frequency_graph_tolerance), 1);
  }

  /* Carry on procesing! Lets check for THE. (copied from affine.c) */
  insertion_trigram_sort(state->trigrams_temp, 17576);
  insertion_digram_sort(state->digrams_temp, 676);

  j = 0; /* All is ok (this should reach 2) */

  /* Check that we have found THE. */
  temp_the = state->trigrams_temp[17572];
  if (temp_the.trigram_ch1 == 19 || temp_the.trigram_ch2 == 7 ||
      temp_the.trigram_ch3 == 4)
  {
    j++;
  }

  /* Check that we can find a TH digram */
  for (i = 675; i >= 665; i--)
  {
    if (state->digrams_temp[i].digram_ch1 == 19 && 
        state->digrams_temp[i].digram_ch2 == 7)
    {
      j++;
      break;
    }
  }

  printf("j: %i\n", j);

  if (j != 2)
  {
    /* Can't find it. Return a score less than 200, based on freq. */
    return max(200 - fg_diff, 1);
  }

  /* OK! All is looking good so far. Ish. Now we're clear to run a full
   * dictionary check on this; returning 200 + score */

  return 200 + score_text_dict_fast(text, state->text_size);
}

int score_text_dict_fast(char *text, int size)
{
  int i, jlen_buf, prefix, match_size, score;
  char *test_start, *test_end, *j;
  char ch1, ch2;

  /* Prepare */
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

    /* Find the smallest possibility (so start high) */
    match_size = WORD_BUF_SIZE;

    for (j = test_start; j < test_end; j += jlen_buf + 1) /* Remember \0 */
    {
      /* In theory, the \0 terminators should take care of all size checks,
       * we use strncmp to limit to checking the correct size of text. */
      jlen_buf = strlen(j);

      if (jlen_buf < match_size && strncasecmp(j, text + i, jlen_buf) == 0)
        match_size = jlen_buf;
    }

    if (match_size != WORD_BUF_SIZE)
    {
      score += match_size;
    }
    else
    {
      /* For the loop incrementing... */
      match_size = 1;
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


