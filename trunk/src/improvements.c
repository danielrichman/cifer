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

void monoalph_ga_crack(char *text, int text_size)
{
  int table[26], width[26];
  int m, i;

  printf("Beginning Monoalphabetic Crack via Genetic Algorithm...\n");
  printf("auto_monalph_parents: auto-creating parents & starting GA\n");

  auto_monoalph_parents(text, text_size, table);
  monoalph_substitute(text, text_size, table);

  /* All the widths will be 2 so no need to bother with the dynamics */
  printf("Printing monalph_ga_crack result: \n");

  printf("Char | NewChar | Char's Number | NewChars Number \n");

  print_setup_width(width, &m);
  print_count_width(table, width, &m);
  print_finalise_width(width, &m);

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(table[i]));
  printf("\n");

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], i);
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], table[i]);
  printf("\n\n");

  printf("%*s\n\n", text_size, text);

  printf("auto_monoalph_parents - done.\n\n");
}

void auto_monoalph_parents(char *text, int text_size, int *target_success)
{
  int frequency_graph[26], identity_frequency_graph[26];
  int parents[30][26];
  digram  digrams[10];
  trigram trigrams[1];
  int i, j;

  /* Start with 1 => 10 as best frequency */
  /* 2 => 20 Random except for THE */
  /* Rest as totally random */

  /* Grab the best frequency match */
  count_freq(text, text_size, frequency_graph);
  create_identity_frequency_graph(identity_frequency_graph, text_size);
  random_frequency_match(frequency_graph, identity_frequency_graph, parents[0]);

  /* Copy into the first ten parents */
  for (i = 1; i < 10; i++) for (j = 0; j < 26; j++) 
          parents[i][j] = parents[0][j];

  /* Load up the rest with randoms */
  setup_random();
  for (i = 10; i < 30; i++) for (j = 0; j < 26; j++)
          parents[i][j] = urandomc(26);

  /* Find THE */
  count_digrams(text, text_size, digrams, 10);
  count_trigrams(text, text_size, trigrams, 1);

  /* See if we can find the right digram, just to double check */
  j = 0;

  for (i = 0; i < 10; i++)
  {
    if (digrams[i].digram_ch1 == trigrams[0].trigram_ch1 &&
        digrams[i].digram_ch2 == trigrams[0].trigram_ch2)
    {
      j = i;
    }
  }

  /* Make sure that the first match for each is TH and THE */
  if (digrams[j].digram_ch1 != trigrams[0].trigram_ch1 ||
      digrams[j].digram_ch2 != trigrams[0].trigram_ch2)
  {
    printf("auto_monoalph_parents: Ambiguous failure: Unable to find THE\n");
    printf("Best digram %c%c: %i, trigram %c%c%c: %i.\n",
          NUMCHAR(digrams[j].digram_ch1), NUMCHAR(digrams[j].digram_ch2),
          digrams[j].digram_value,
          NUMCHAR(trigrams[0].trigram_ch1), NUMCHAR(trigrams[0].trigram_ch2),
          NUMCHAR(trigrams[0].trigram_ch3),
          trigrams[0].trigram_value);

    return;
  }

  /* Load out 10 => 20 with THE filled in (ctext => 19, 7, 4) */
  for (i = 10; i < 20; i++) parents[i][trigrams[0].trigram_ch1] = 19;
  for (i = 10; i < 20; i++) parents[i][trigrams[0].trigram_ch2] = 7;
  for (i = 10; i < 20; i++) parents[i][trigrams[0].trigram_ch3] = 4;

  /* Ok. That's enough randomness & logic combinations. Start the GA */
  monoalph_imp_genetic(parents, text, text_size, target_success);

  /* The target will have been loaded out with the result, so all is done*/
}

/* Monoalphabetic, (keyword, affine, caesar) improvements via
 * a genetic algorithm, using score_text as its evaluator
 * THIS FUNCTION REQUIRES 30 PARENTS of array[cipher] => plain */
void monoalph_imp_genetic(int parents[30][26], char *text, int text_size, 
                             int *target_success)
{
  int i, j, i2, i3, s, t, u;
  ga_parent_list sorter[30];
  char *text_temp;
  int *parenta, *parentb;
  int overall_best, depeak_count;
  int children[30][26];
  int overall_best_child[26];
  int generation;

  /* Setup the temporary text "the breeding ground" */
  text_temp = malloc(text_size);
  if (text_temp == NULL)
  {
    printf("monoalph_imp_genetic: text_temp malloc(%i) failed.\n", text_size);
    exit(1);
  }

  /* Setup the urandom */
  setup_random();

  /* Setup our variables */
  overall_best = 0;
  depeak_count = 0;

  /* Say that the loop is starting */
  printf("GA Starting...");
  fflush(stdout);

  /* Enter the cycle. */
  for (generation = 0; generation < 100 && depeak_count < 6; generation++)
  {
    /* Step (1) Determine the fittest. */
    for (i = 0; i < 30; i++)
    {
      memcpy(text_temp, text, text_size);
      monoalph_substitute(text_temp, text_size, parents[i]);

      sorter[i].parent_id = i;
      sorter[i].parent_score = score_text_dict_fast(text_temp, text_size);
    }

    ga_natural_selection(sorter, 30);

    /* Now the best parent is in sorter[29] and worst in [0] */

    /* Track the best, and keep track of when we're not getting any better */
    if (sorter[29].parent_score > overall_best)
    {
      overall_best = sorter[29].parent_score;
      for (i = 0; i < 26; i++) 
             overall_best_child[i] = parents[sorter[29].parent_id][i];
      depeak_count = 0;
    }
    else
    {
      depeak_count++;
    }

    /* Mate 29 with 28 to produce 3 children, 1&2 are crossovers, the
     *  final one is a 3 way mix between parenta, parentb & random. */
    /* To make sorting faster next time, we guess and put the children of the
     * higher parents into the higher new slots, right? */

    for (i = 0; i < 10; i++)
    {
      i2 = i * 2;   i3 = i * 3;

      /* Find and point to the parents */
      parenta = parents[sorter[29 - i2].parent_id];
      parentb = parents[sorter[28 - i2].parent_id];

      /* Create the children */
      /* Child 1: Simple crossover. Even = parenta, odd parentb */
      for (j = 0; j < 26; j += 2) children[29 - i3][j] = parenta[j];
      for (j = 1; j < 26; j += 2) children[29 - i3][j] = parentb[j];
      /* Child 2: Reverse of Child 1 */
      for (j = 0; j < 26; j += 2) children[28 - i3][j] = parentb[j];
      for (j = 1; j < 26; j += 2) children[28 - i3][j] = parenta[j];
      /* Child 3: Mix of Parenta, parentb and random */
      s = urandomc(3);
      t = urandomc(3);
      if (t == s)   t = (t + 1) % 3;
      /* Adding s and t together shows us which numbers they are using quickly
       * if they are taking 0 and 1 it returns 1 so we take 2
       *                    0     2            2            1
       *                    1     2            3            0 */
      u = 3 - (s + t);
      for (j = s; j < 26; j += 3) children[27 - i3][j] = parenta[j];
      for (j = t; j < 26; j += 3) children[27 - i3][j] = parentb[j];
      for (j = u; j < 26; j += 3) 
               children[27 - i3][j] = urandomc(26);
    }

    /* Loop around again */
  }

  /* Say what was the overall result */
  printf("Done: Generation %i reached, Depeak %i, OverallBest %i\n",
                             generation, depeak_count, overall_best);

  /* Free up the breeding ground */
  free(text_temp);

  /* Copy the best match into the target array */
  for (i = 0; i < 26; i++) target_success[i] = overall_best_child[i];

  /* Done! */
}
