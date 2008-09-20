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
void action_do(char **argv)     /* TODO: see main.c */
{
  int frequency_graph[26];
  /* digram digrams[5] trigram trigrams[5] (Only get top five) */

  printf("Doing requested action(s)...\n");
 
  if (arg_input) /* Have we got any input files? */
  {
    input_open(arg_input_argc, argv);
  }
 
  /* Let's start with the easy stuff */
  
  if (arg_pct) /* Print char -> number table */
  {
    printf("Char -> number table requested... printing...\n");
    print_char_table();
  }
  
  if (arg_freq) /* Do (and print) letter frequency count */
  { /* TODO: DON'T FREQCOUNT WITHOUT AN INPUT FILE! */
    printf("Letter frequency count requested... doing...\n");
    count_freq(intext, intext_size, frequency_graph);
    print_freq(frequency_graph);
  }
  
  /* Ciphers */
  
  if (arg_vig) /* Crack vigenere */
  {
    printf("Vigenere cipher crack requested... doing...\n");
    crack_vigenere(intext, intext_size);
  }
  
  printf("Finished doing requested actions.\n");
}
