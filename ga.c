#include <stdio.h>
#include <stdlib.h>

#define GA_POPSIZE 2048 // Population size
#define GA_MAXITER 16348 // Maximum iterations
#define GA_ELITERATE 0.10 // Elitism rate
#define GA_MUTPERC 0.25 // Mutation rate
#define PRINT_INTER 1000 // Print status every this iterations/generations

#define GA_ELITSIZE (GA_POPSIZE * GA_ELITERATE) // Number of elites
#define GA_MUTCHANCE 4

#define TARGET_LEN 20
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
void print_best(ga_memb *pop);
void mate_pop(ga_memb *pop, ga_memb *buf); // Mates pop into buf
void mutate(ga_memb *pop); // Mutates some of the population

int main(void)
{
  int i, j;
  
  ga_memb pop_a[GA_POPSIZE], pop_b[GA_POPSIZE];
  ga_memb *pop, *buf, *tmp;
  
  pop = pop_a;
  buf = pop_b;
  
  init_pop(pop_a);
  init_pop(pop_b);
  
  for (i = 0, j = 0; i < GA_MAXITER; i++, j++)
  {
    calc_fitness(pop); // Fill out ga_memb.fitness
    sort_by_fitness(pop);
    if (j > PRINT_INTER) print_best(pop);
    
    if (pop[0].fitness == TARGET_LEN) break; // We can has solution! :)
    
    // mate_pop(*pop, *buf) // Mate the population into the buffer
    // mutate(*buf) // Radiation!
    
    // Swap the pointers
    tmp = buf; // For some reason gcc doesn't like XOR Swap ownage on 
    buf = pop; // a pointer ='( - so we can use temp variable.
    pop = tmp;
  }

  print_best(pop);

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
  int i;
  int j;
  for (i = 0; i < GA_POPSIZE; i++)
  {
    for (j = 0; j < TARGET_LEN; j++)
    {
      pop[i].sol[j] = rand();
    }
  }
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

// Insertion sort
void sort_by_fitness(ga_memb *pop)
{
  int i, j;
  int value;
  
  for (i = 0; i < GA_POPSIZE; i++)
  {
    value = pop[i].fitness;
    j = i - 1;
    while ((j >= 0) && (pop[i].fitness > value))
    {
      pop[j + 1] = pop[j];
      j -= 1;
    }
    pop[j + 1].fitness = value;
  }
}

void print_best(ga_memb *pop)
{
  int i;

  printf("Best: %d", *(pop[0].sol));
  for (i = 1; i < TARGET_LEN; i++) printf(", %d", *((pop[0].sol) + i));
  printf("  (%d%%)\n", (pop[0].fitness / TARGET_LEN) * 100);
}

void mate_pop(ga_memb *pop, ga_memb *buf) // Mates pop into buf
{
  int i;
  for (i = 0; i < GA_ELITSIZE; i++)
  {
    buf[i].

    if (rand() % GA_MUTCHANCE == 0)  mutate(buf + i);
  }
}

void mutate(ga_memb *pop) // Mutates some of the population
{
  /* Radiation babies */
  *(pop).sol[ rand() % TARGET_LEN ] = rand();
}