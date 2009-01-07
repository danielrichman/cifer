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

/* TODO: Make the random frequency match function (bruteforce) try various
 * combinations of suggestions in the sorted array (removing one option,
 * then two, sliding about, etc.) and score the results, for better monoalph
 * craxin
 * Perhaps we could have some genetic algorithm ownage here? */

/* This function takes two graphs and tries to generate the best match
 * table. (table being array[source] => identity or array[cipher] => plain */
void random_frequency_match(int *frequency_graph,
                            int *identity_frequency_graph,
                            int *table)
{
  /* In order to take into account all the crossmatching and consider
   * all the data in one go, we build up a struct array of diffs, scores
   * sources and targets; then sort & extract. Tastey. */

  /* stack variables */
  int i, j, k;
  rand_freq scores[26 * 26];

  /* Load up the structs */
  for (i = 0; i < 26; i++)  for (j = 0; j < 26; j++)
  {
    k = (i * 26) + j;
    scores[k].source = i;
    scores[k].target = j;
    scores[k].diff = diff(frequency_graph[i], identity_frequency_graph[j]);
  }

  /* Sort. */
  insertion_sort_rand_freq(scores, 26 * 26);

  /* Zero the table */
  for (i = 0; i < 26; i++) table[i] = -1;

  /* Extract the information. */
  for (i = 0; i < (26 * 26); i++)
  {
    /* If the source is not taken... */
    if (table[scores[i].source] == -1)
    {
      /* Check if the target is taken */
      k = 1;
      for (j = 0; j < 26; j++)  if (table[j] == scores[i].target)  k = 0;

      /* And save */
      if (k)
      {
        table[scores[i].source] = scores[i].target;
      }
    }
  }

  /* Done */
}

void frequency_guesses(char *intext, int intext_size, 
                       char *outtext, int do_decrypt)
{
  int frequency_graph[26], identity_frequency_graph[26], gid_freq[26];
  int table[26], rtable[26], width[26];
  int i, m, t, c;
  char o;

  /* Count */
  count_freq(intext, intext_size, frequency_graph);

  /* Because only alpha chars are counted, we need a new total */
  t = 0;
  for (i = 0; i < 26; i++) t += frequency_graph[i];

  /* Now identity frequency graph time */
  create_identity_frequency_graph(identity_frequency_graph, t);

  /* Process */
  random_frequency_match(frequency_graph, identity_frequency_graph, table);

  /* Setup the guessed plainchar frequency information */
  for (i = 0; i < 26; i++)
  {
    gid_freq[i] = identity_frequency_graph[table[i]];
  }

  /* Print out the information we gathered */
  printf("Charnums and Frequencies: \n");

  print_setup_width(width, &m);
  print_count_width(frequency_graph, width, &m);
  print_count_width(identity_frequency_graph, width, &m);
  print_count_width(table, width, &m);
  print_count_width(gid_freq, width, &m);
  print_finalise_width(width, &m);

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], i);
  printf("\n");

  printf("F|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], frequency_graph[i]);
  printf("\n");

  printf("I|");
  for (i = 0; i < 26; i++)
        printf("%*i|", width[i], identity_frequency_graph[i]);
  printf("\n\n");

  printf("Guessed Letters, ctext => ptext: \n");

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(i));
  printf("\n");

  printf("G|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(table[i]));
  printf("\n");

  printf("Guessed letters, ptext => ctext: \n");

  for (i = 0; i < 26; i++) rtable[table[i]] = i;

  printf("G|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(i));
  printf("\n");

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(rtable[i]));
  printf("\n\n");

  if (do_decrypt)
  {
    printf("*Possible* but unlikely decrypt based purely on letters: \n");

    for (i = 0; i < intext_size; i++)
    {
      o = *(intext + i);
      c = CHARNUM(o);

      if (c != -1)           o = NUMCHAR(table[c]);
      if (outtext != NULL)   *(outtext + i) = o;

      printf("%c", o);
    }

    printf("\n\n");
  }
}

/* This frequency counter & analyser is different in that it incorporates
 * jumps; ie, columns */
int frequency_analysis(char *text, int text_size, int jump)
{
  int frequency_graph[26];
  int identity_frequency_graph[26];
  int i, j, h, ch, temp_diff, best_diff, best_diff_shift;

  /* Prepare the variables */
  best_diff = -1;
  best_diff_shift = 0;
  j = 0;
  h = 0;

  /* Zero the freq. graph and then scan the text */
  for (i = 0; i < 26; i++)  frequency_graph[i] = 0;

  /* J is used to temporarily store the total text/column size */
  /* H is used to count up to a jump */
  i = 0;
  while (i < text_size)
  {
    ch = CHARNUM(*(text + i));
    i++;

    if (ch != -1)
    {
      if (modp(h, jump) == 0)
      {
        frequency_graph[ch] += 1;
        j++;
      }

      h++;
    }
  }

  /* Setup the identity freq graphs */
  create_identity_frequency_graph(identity_frequency_graph, j);

  for (i = 0; i < 26; i++)
  {
    temp_diff = 0;

    /* Compare the new letters against the identity */
    for (j = 0; j < 26; j++)
    {
      temp_diff += diff(frequency_graph[modp(j + i, 26)],
                        identity_frequency_graph[j]);
    }

    /* And keep track of the best match */
    if (best_diff == -1 || temp_diff < best_diff)
    {
      best_diff = temp_diff;
      best_diff_shift = i;
    }
  }

  return best_diff_shift;
}

double delta_ic (char *text, int text_size, int jump)
{
  int sfreq[26];
  int i, t;
  double j;

  j = 0; t = 0;

  /* Zero the char freq array; then count up the char freqs, then 
   * calculate (3rd for) the top half of the fraction, 4 - total the chars 
   * counted, finally, do the dividing from the bottom half */

  for (i = 0; i < 26; i++)                sfreq[i] = 0;
  for (i = 0; i < text_size; i += jump)   sfreq[CHARNUM(*(text + i))] += 1;
  for (i = 0; i < 26; i++)                j += sfreq[i] * (sfreq[i] - 1);
  for (i = 0; i < 26; i++)                t += sfreq[i];

  j = j / (t * (t - 1) / 26);

  return j;
}


