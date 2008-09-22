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
  int i;

  /* Grab the best frequency match */
  count_freq(text, text_size, frequency_graph);
  create_identity_frequency_graph(identity_frequency_graph, text_size);
  random_frequency_match(frequency_graph, identity_frequency_graph, parents[0]);

  /* Load up the rest with randoms */
  setup_random();
  for (i = 1; i < 30; i++) ga_monoalph_create_random(parents[i]);

  /* Start the GA */
  monoalph_imp_genetic(parents, text, text_size, target_success);

  /* The target will have been loaded out with the result, so all is done*/
}

/* Monoalphabetic, (keyword, affine, caesar) improvements via
 * a genetic algorithm, using score_text as its evaluator
 * THIS FUNCTION REQUIRES 30 PARENTS of array[cipher] => plain */
void monoalph_imp_genetic(int parents[30][26], char *text, int text_size, 
                             int *target_success)
{
  int i, j, i2, i3, s;
  ga_parent_list sorter[30];
  char *text_temp;
  int *parenta, *parentb;
  int overall_best, depeak_count;
  int children[30][26], temp_rand[26];
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
  for (generation = 0; generation < 1000 && depeak_count < 100; generation++)
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
      ga_monoalph_create_child(parenta, parentb, children[29 - i3]);
      /* Child 2: Reverse of Child 1 */
      ga_monoalph_create_child(parentb, parenta, children[28 - i3]);
      /* Child 3: Mix of Parenta, parentb and random */
      s = urandomc(2);
      ga_monoalph_create_random(temp_rand);
      if (s == 0) 
         ga_monoalph_create_child(parenta, temp_rand, children[27 - i3]);
      else
         ga_monoalph_create_child(parentb, temp_rand, children[27 - i3]);
    }

    /* Time for the children to grow up */
    for (i = 0; i < 30; i++) for (j = 0; j < 26; j++) 
            parents[i][j] = children[i][j];

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

void ga_monoalph_create_random(int *target)
{
  int i, j, r, choosing;

  for (i = 0; i < 26; i++) target[i] = -1;

  for (i = 0; i < 26; i++)
  {
    choosing = 1;

    /* Find a char that hasn't yet been taken */
    while (choosing)
    {
      r = urandomc(26); /* Pick one */
      choosing = 0;

      for (j = 0; j < 26; j++)
      {
        if (target[j] == r) choosing = 1; /* Pick another try */
      }
    }

    /* Assign it to i */
    target[i] = r;
  }
}

void ga_monoalph_create_child(int *parenta, int *parentb, int *child)
{
  int i, j, parenta_cc, parentb_cc;
  int choice1, choice2, choice1p, getting_choice;
  int attempt, attempt_ok;

  parenta_cc = 0; parentb_cc = 0;

  for (i = 0; i < 26; i++) child[i] = -1;

  for (i = 0; i < 26; i++)
  {
    if (parenta_cc > parentb_cc)
    {
      /* The next, if possible, should be from parentb */
      choice1 = parentb[i];
      choice2 = parenta[i];
      choice1p = 0;
    }
    else
    {
      choice1 = parenta[i];
      choice2 = parentb[i];
      choice1p = 1;
    }

    getting_choice = 1;
    while (getting_choice)
    {
      if (getting_choice == 1)
      {
        attempt = choice1;
      }
      else if (getting_choice == 2)
      {
        attempt = choice2;
      }
      else
      {
        attempt = urandomc(26);
      }

      attempt_ok = 1;
      for (j = 0; j < 26; j++)
      {
        if (child[j] == attempt)
        {
          getting_choice++;
          attempt_ok = 0;
        }
      }

      if (attempt_ok)
      {
        if (choice1p == 1 && getting_choice == 1) parenta_cc++;
        if (choice1p == 1 && getting_choice == 2) parentb_cc++;
        if (choice1p == 0 && getting_choice == 1) parentb_cc++;
        if (choice1p == 0 && getting_choice == 2) parenta_cc++;

        child[i] = attempt;
        getting_choice = 0;
      }
    }
  }
}
