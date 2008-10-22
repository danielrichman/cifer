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

#include <stdio.h>
#include <stdlib.h>

#define GA_POPSIZE 2048 // Population size
#define GA_MAXITER 16348 // Maximum iterations
#define GA_ELITERATE 0.10 // Elitism rate
#define GA_MUTPERC 0.25 // Mutation rate
#define PRINT_INTER 1 // Print status every this iterations/generations

#define GA_ELITSIZE (GA_POPSIZE * GA_ELITERATE) // Number of elites
#define GA_MUTCHANCE 4

#define TARGET_LEN       20
#define CHROMOSOME_MAX   500
int target[TARGET_LEN] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                           0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // Target solution

typedef struct
{
  int sol[TARGET_LEN]; // This member's solution (chromosome)
  int fitness;         // How good the solution is
} ga_memb;

void init_pop(ga_memb *pop);
  void zero_fitness(ga_memb *pop);
  void randall_sols(ga_memb *pop);
void calc_fitness(ga_memb *pop);
void sort_by_fitness(ga_memb *pop);
void print_best(ga_memb *pop, unsigned const int gen);
void mate_pop(ga_memb *pop, ga_memb *buf); // Mates pop into buf
  void cp_mems(ga_memb *src, ga_memb *targ, unsigned int size);
  void mutate(ga_memb *pop); // Mutates some of the population
void swap_pts(ga_memb **pt1, ga_memb **pt2);

#define sort_by_fitness(a) (insertion_sort_ga_memb(a, GA_POPSIZE))
void insertion_sort_ga_memb(ga_memb *a, int asize);

int main(void)
{
  int i, j;
  
  ga_memb pop_a[GA_POPSIZE], pop_b[GA_POPSIZE];
  ga_memb *pop, *buf;
 
  srand(time());
 
  pop = pop_a;
  buf = pop_b;
  
  init_pop(pop_a);
  init_pop(pop_b);
  
  for (i = 0, j = 0; i < GA_MAXITER; i++, j++)
  {
    calc_fitness(pop); // Fill out ga_memb.fitness
    sort_by_fitness(pop);
    
    if (j > PRINT_INTER)
    {
      print_best(pop, i);
      j = 0;
    }
    
    // We can has solution! :)
    if (pop[GA_POPSIZE - 1].fitness == TARGET_LEN) break;
    
    mate_pop(pop, buf); // Mate the population into the buffer
    swap_pts(&pop, &buf);
  }

  print_best(pop, i);

  return 0;
}

void init_pop(ga_memb *pop)
{
  zero_fitness(pop);
  randall_sols(pop);
}

void zero_fitness(ga_memb *pop)
{
  int i;
  for (i = 0; i < GA_POPSIZE; i++)
  {
    pop[i].fitness = 0;
  }
}

void randall_sols(ga_memb *pop)
{
  int i, j;
  for (i = 0; i < GA_POPSIZE; i++)  for (j = 0; j < TARGET_LEN; j++)
    pop[i].sol[j] = rand() % CHROMOSOME_MAX;
}

void calc_fitness(ga_memb *pop)
{
  unsigned int fitness;
  
  int i, j;
  for (i = 0; i < GA_POPSIZE; i++)
  {
    fitness = 0;
    for (j = 0; j < TARGET_LEN; j++)
    {
      if (pop[i].sol[j] == target[j])
      {
        fitness += 1;
      }
    }
    pop[i].fitness = fitness;
  }
}

#define balh(a) (insertion_sort_ga_memb(a, GA_POPSIZE))

void insertion_sort_ga_memb(ga_memb *a, int asize)
{
  int i, j, k;
  ga_memb d;

  k = asize;
  for (i = 0; i < k; i++)
  {
    d = a[i];
    j = i - 1;
    while (j >= 0 && a[j].fitness > d.fitness)
    {
      a[j + 1] = a[j];
      j = j - 1;
    }
    a[j + 1] = d;
  }
}

void print_best(ga_memb *pop, unsigned const int gen)
{
  int i;

  printf("At gen %d, best: %d", gen, *(pop[GA_POPSIZE - 1].sol));
  for (i = 1; i < TARGET_LEN; i++) 
              printf(", %d", *((pop[GA_POPSIZE - 1].sol) + i));
  printf("  (%d%%)\n", (pop[GA_POPSIZE - 1].fitness * 100 )/ TARGET_LEN);
}

void mate_pop(ga_memb *pop, ga_memb *buf) // Mates pop into buf
{
  unsigned int i, j;
  int randint;
  
  // Copy over the elites
  cp_mems(pop, buf, GA_ELITSIZE);
  
  // Don't touch the elites, mate the others
  for (i = GA_ELITSIZE; i < GA_POPSIZE; i += 2)
  {
    randint = rand() % TARGET_LEN;
    
    // The first half of the chromosomes don't changef
    for (j = 0; j < randint; j++)
    {
      buf[i    ].sol[j] = pop[i    ].sol[j];
      buf[i + 1].sol[j] = pop[i + 1].sol[j];
    }
    // The second half of the chromosomes are swapped
    for (j = randint; j < TARGET_LEN; j++)
    {
      buf[i    ].sol[j] = pop[i + 1].sol[j];
      buf[i + 1].sol[j] = pop[i    ].sol[j];
    }
  }
  
  mutate(buf);
}

// Swap pt1 and pt2
void swap_pts(ga_memb **pt1, ga_memb **pt2)
{
  ga_memb *pt_tmp;
  
  pt_tmp = *pt2;
  *pt2 = *pt1;
  *pt1 = pt_tmp;
}

// Mutates some of the population
void mutate(ga_memb *pop)
{
  int i, randint;
  for (i = 0; i < GA_ELITSIZE; i++)
  {
    if (rand() % GA_MUTCHANCE == 0)
    {
      randint = rand();
      pop[0].sol[randint % TARGET_LEN] = randint;
    }
  }
}

void cp_mems(ga_memb *src, ga_memb *targ, unsigned int size)
{
  unsigned int i, j;
  
  for (i = GA_POPSIZE - 1; i >= size; i--)
  {
    targ[i].fitness = src[i].fitness;
    for (j = 0; j < TARGET_LEN; j++)
    {
      targ[i].sol[j] = src[i].sol[j];
    }
  }
}
