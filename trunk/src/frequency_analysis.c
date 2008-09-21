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
  insertion_randfreq_sort(scores, 26 * 26);

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

void frequency_guesses(char *text, int text_size)
{
  int frequency_graph[26], identity_frequency_graph[26], gid_freq[26];
  int table[26], width[26];
  int i, m;

  /* Count */
  count_freq(text, text_size, frequency_graph);
  create_identity_frequency_graph(identity_frequency_graph, text_size);

  /* Process */
  random_frequency_match(frequency_graph, identity_frequency_graph, table);

  /* Setup the guessed plainchar frequency information */
  for (i = 0; i < 26; i++)
  {
    gid_freq[i] = identity_frequency_graph[table[i]];
  }

  /* Print out the information we gathered */
  printf("Frequency Guesses:\n");

  printf("Char | Char's Number | Char's Frequency | Char's Id-Freq | \n");
  printf("  Guessed PlainChar  | PlainChar's  num | PlainChar's ID-freq\n\n");

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
  printf("\n");

  printf("G|");
  for (i = 0; i < 26; i++)
        printf("%*i|", width[i], identity_frequency_graph[table[i]]);
  printf("\n\n");
}

/* This frequency counter & analyser is different in that it incorporates
 * jumps; ie, columns */
int frequency_analysis(char *text, int text_size, int jump)
{
  int frequency_graph[26];
  int identity_frequency_graph[26];
  int i, j, temp_diff, best_diff, best_diff_shift;

  /* Prepare the variables */
  best_diff = -1;
  best_diff_shift = 0;
  j = 0;

  /* Zero the freq. graph and then scan the text */
  for (i = 0; i < 26; i++)  frequency_graph[i] = 0;

  /* J is used to temporarily store the total text/column size */
  for (i = 0; i < text_size; i += jump)
  {
    frequency_graph[CHARNUM(*(text + i))] += 1;
    j++;
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


