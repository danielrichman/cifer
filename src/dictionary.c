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

// TODO: Add Unload_dict & action (or add to load_dict) so that dictionaries can
// be swapped, ie, user wants something different to default dictionary

char *dict;
char *dict_location;
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

  /* Pre malloc_good all the space needed */
  state->text_size = text_size;
  state->frequency_graph_tolerance = text_size / 3;
  state->frequency_graph          = malloc_good( sizeof(int) * 26 );
  state->identity_frequency_graph = malloc_good( sizeof(int) * 26 );
  state->digrams_temp  = malloc_good( sizeof(digram)  * 26 * 26 );
  state->trigrams_temp = malloc_good( sizeof(trigram) * 26 * 26 * 26 );

  /* Setup id. frequency graph */
  create_identity_frequency_graph(state->identity_frequency_graph, text_size);

  /* Setup the counters */
  state->num_checked = 0;
  state->num_checked_freq_ok = 0;
  state->num_checked_the_ok = 0;

  /* So that any function knows its all good */
  state->init_check    = DICT_INIT_CHECK;
}

void score_text_pro_cleanup(score_text_pro_state *state)
{
  free(state->digrams_temp);
  free(state->trigrams_temp);
  free(state->frequency_graph);
  free(state->identity_frequency_graph);
}

void score_text_pro_print_stats(char *englishname, score_text_pro_state *state)
{
  printf("Score text pro '%s' stats:\n", englishname);
  printf("  Number of Texts checked:                           %i\n", 
                  state->num_checked);
  printf("  Number of Texts checked with ok frequency_graph:   %i (tol. %i)\n", 
                  state->num_checked_freq_ok, state->frequency_graph_tolerance);
  printf("  Number of Texts checked with ok 'THE' location:    %i\n\n", 
                  state->num_checked_the_ok);
}

int score_text_pro(char *text, score_text_pro_state *state)
{
  int i, j, k, h, l, fg_diff, temp_th_num;
  digram temp_th;
  trigram temp_the;

  /* This routine is made to quickly discard garbage but generate a better
   * score for real matches */

  if (state->init_check == DICT_INIT_CHECK)  return 0;
  state->num_checked ++;

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
    h =  CHARNUM(*(text + i + 1));
    k =  CHARNUM(*(text + i));

    if (j != -1 && h != -1 && k != -1)    
    {
      h = (h * 26)  + j;
      k = (k * 676) + h;
      state->trigrams_temp[k].trigram_value ++;
      state->digrams_temp[h].digram_value   ++;
      state->frequency_graph[j]             ++;
    }
  }

  i = CHARNUM(*text);
  j = CHARNUM(*(text + 1));

  if (i != -1 && j != -1)
  {
    h = (i * 26) + j;

    state->digrams_temp[h].digram_value ++;
    state->frequency_graph[i] ++;
    state->frequency_graph[j] ++;
  }

  /* Generate some sort of "diff" for the frequency graph */
  for (i = 0; i < 26; i++)
  {
    fg_diff += diff(state->frequency_graph[i], 
                    state->identity_frequency_graph[i]);
  }

  /* Does it meet the cutoff ? */
  if (fg_diff > state->frequency_graph_tolerance)
  {
    /* Missed the cutoff; return a score < 100 */
    return max(100 - (fg_diff - state->frequency_graph_tolerance), 1);
  }

  state->num_checked_freq_ok ++;

  /* Carry on procesing! Lets check for THE. (copied from affine.c) */
  temp_the = best_trigram(state->trigrams_temp, 17576);

  j = 0; /* All is ok (this should reach 3) */

  /* Check that we have found THE. */
  if (temp_the.trigram_ch1 == 19 || temp_the.trigram_ch2 == 7 ||
      temp_the.trigram_ch3 == 4)
  {
    j += 1;
  }

  /* Check that we can find a TH digram */
  for (i = 0; i < 10; i++)
  {
    /* Get a good digram (the best, actually) */
    temp_th_num = best_digram_key(state->digrams_temp, 676);
    temp_th = state->digrams_temp[temp_th_num];

    if (temp_th.digram_ch1 == 19 && 
        temp_th.digram_ch2 == 7)
    {
      j += 2;
      break;
    }

    /* Fail? Remove that digram by setting it to 0, then get the next best */
    state->digrams_temp[temp_th_num].digram_value = 0;
  }

  if (j != 3)
  {
    /* Can't find it. Return a score less than 200, based on freq. */
    return max(200 - fg_diff, 1);
  }

  state->num_checked_the_ok ++;

  /* OK! All is looking good so far. Ish. Now we're clear to run a full
   * dictionary check on this; returning 200 + score */

  return 200 + score_text_dict_fast(text, state->text_size);
}

int score_text_dict_fast(char *text, int size)
{
  int i, jlen_buf, prefix, match_size, score;
  char *test_start, *test_end, *j;
  char ch1, ch2;

  /* Check */
  if (dict == NULL)          return 0; 

  /* Prepare */
  match_size = 1;
  score = 0;

  /* If it gets to size - MIN_WORD_SIZE we won't find the last bit anyway 
   * because it's limited by MIN_WORD_SIZE */

  for (i = 0; i < (size - MIN_WORD_SIZE); i += max(match_size, 1))
  {
    ch1 = *(text + i);
    ch2 = *(text + i + 1);
    match_size = 0;

    if (ALPHA_CH(ch1) && ALPHA_CH(ch2))
    {
      prefix = (CHARNUM( ch1 ) * 26) + CHARNUM( ch2 );

      test_start = *(dict_pointer + prefix);
      test_end = *(dict_pointer_end + prefix);

      for (j = test_start; j < test_end; j += jlen_buf + 1) /* Remember \0 */
      {
        /* In theory, the \0 terminators should take care of all size checks,
         * we use strncmp to limit to checking the correct size of text. */
        jlen_buf = strlen(j);

        if (jlen_buf > match_size && strncasecmp(j, text + i, jlen_buf) == 0)
          match_size = jlen_buf;
      }

      score += match_size;
    }
  }

  return score;
}

void score_text_dict_spaces(char *text, int size, int *space_array)
{
  int i, jlen_buf, prefix, match_size, super_size, failing;
  char *test_start, *test_end, *j;
  char ch1, ch2;

  if (dict == NULL)          return;

  match_size = 1;
  failing = 0;

  for (i = 0; i < size; i++)  *(space_array + i) = 0;

  for (i = 0; i < (size - MIN_WORD_SIZE); i += max(match_size, 1))
  {
    ch1 = *(text + i);
    ch2 = *(text + i + 1);
    match_size = 0;
    super_size = 0;

    /* For spacing, we don't care about speed or score, so we use
     * this hard coded preference list to make it readable */
    #define superword(w)  if (strncasecmp(w, text + i, sizeof(w) - 1) == 0) \
                                            super_size = sizeof(w) - 1;

    if (ALPHA_CH(ch1) && ALPHA_CH(ch2))
    {
           superword("the")
      else superword("of")
      else superword("to")
      else superword("in")
      else superword("and")
      else superword("for")
      else superword("was")
      else superword("is")
      else superword("that")
      else superword("on")
      else superword("at")
      else superword("he")
      else superword("with")
      else superword("by")
      else superword("be")
      else superword("it")
      else superword("an")
      else superword("his")

      match_size = super_size;
      prefix = (CHARNUM( ch1 ) * 26) + CHARNUM( ch2 );

      test_start = *(dict_pointer + prefix);
      test_end = *(dict_pointer_end + prefix);

      for (j = test_start; j < test_end; j += jlen_buf + 1)
      {
        jlen_buf = strlen(j);

        if (jlen_buf > match_size && strncasecmp(j, text + i, jlen_buf) == 0)
          match_size = jlen_buf;
      }

      /* Something like this its worth hinting in super's favour */
      if (match_size - 1 == super_size)  match_size = super_size;

      if (match_size != 0)
      {
        if (failing)  *(space_array + i - 1) = 1;
        *(space_array + i + match_size - 1) = 1;
        failing = 0;
      }
      else
      {
        failing = 1;
      }
    }
  }
}

void init_dict(void)
{
  /* Simply initialises the pointers */
  dict = NULL;
  dict_pointer = NULL;
  dict_pointer_end = NULL;

  /* Set default location */
  dict_location = malloc_good(strlen(DICTIONARY) + 1);
  memcpy(dict_location, DICTIONARY, strlen(DICTIONARY));
  *(dict_location + strlen(DICTIONARY)) = 0;
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

  /* Check */
  if (dict != NULL)  return;

  dictf = fopen(dict_location, "r");
  if (dictf == NULL)
  {
    perror("load_dict: fopen");
    printf(dict_not_loaded);
    dict = NULL;
    return;
  }

  /* Count filesize */
  filesize = 0;
  buf_size = 0;

  while (feof(dictf) == 0)
  {
    ch = getc(dictf);
    if (ALPHA_CH(ch))
    {
      buf_size++;

      if (buf_size >= WORD_BUF_SIZE)
      {
        printf("Error: A Word busted WORD_BUF_SIZE %i\n", WORD_BUF_SIZE);
        printf(dict_not_loaded);
        dict = NULL;
        return;
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

  dict             = malloc_good( filesize + 1 );
  dict_pointer     = malloc_good( sizeof(char *) * ((26 * 26) + 1) );
  dict_pointer_end = malloc_good( sizeof(char *) * ((26 * 26) + 1) );

  dict_insert = dict;
  buf_size = 0;

  printf("Loading dictionary of %i bytes... ", filesize);
  fflush(stdout);

  /* Read it in */
  i = 0;

  while (feof(dictf) == 0)
  {
    ch = getc(dictf);

    if (ALPHA_CH(ch))
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
          printf("\n");
          printf("*dict overflow at %i (Attempted to add %i)\n", i, buf_size);
          printf(dict_not_loaded);

          free(dict);
          free(dict_pointer);
          free(dict_pointer_end);

          dict = NULL;
          return;
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
      printf(dict_not_loaded);

      free(dict);
      free(dict_pointer);
      free(dict_pointer_end);

      dict = NULL;
      return;
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


