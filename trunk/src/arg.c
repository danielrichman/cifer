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

char *arg_input = NULL;  /* !NULL = load this file as input */
int arg_freq  = 0;       /* Do frequency analysis */
int arg_pct   = 0;       /* Print char->number table */
int arg_vig   = 0;       /* Do a vigenere crack */
int arg_aff   = 0;       /* Do an affine crack */
int arg_gcd   = 0;       /* Do a GCD Calculation */
int arg_gcd_1 = 0;
int arg_gcd_2 = 0;
int arg_pt    = 0;       /* Print trigram frequencies */

int arg_parse(int argc, char **argv)
{
  int i;
  int got_arg = 0;
  
  printf("Parsing command line arguments... ");
  
  for (i = 1; i < argc; i++)
  {
    got_arg = 0;

    /* Do frequency analysis */
    if (((strcmp(argv[i], "-f")) == 0) || ((strcmp(argv[i], "--freq") == 0)))
    {
      arg_freq = 1;
      printf("arg_freq, ");
      got_arg = 1;
    }
    
    /* Input file */
    if (((strcmp(argv[i], "-i")) == 0) || ((strcmp(argv[i], "--input") == 0)))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad\n");
        return -1;
      }

      printf("arg_input %s, ", argv[i+1]);
      arg_input = argv[i + 1];
      i++; /* We skip the next argument; it is a filename */
      got_arg = 1;
    }
    
    /* Print a table of mappings from chars to numbers */
    if (((strcmp(argv[i], "-pct")) == 0) ||
        ((strcmp(argv[i], "--print-char-table") == 0)))
    {
      arg_pct = 1;
      printf("arg_pct, ");
      got_arg = 1;
    }
    
    if ((strcmp(argv[i], "-vig")) == 0 ||
       ((strcmp(argv[i], "--vigenere") == 0)))
    {
      arg_vig = 1;
      printf("arg_vig, ");
      got_arg = 1;
    }

    if ((strcmp(argv[i], "-aff")) == 0 ||
       ((strcmp(argv[i], "--affine") == 0)))
    {
      arg_aff = 1;
      printf("arg_vig, ");
      got_arg = 1;
    }


    if ((strcmp(argv[i], "-gcd")) == 0)
    {
      if (i + 2 >= argc)
      {
        printf("arg_bad\n");
        return -1;
      }

      arg_gcd = 1;
      arg_gcd_1 = atoi(argv[i + 1]);
      arg_gcd_2 = atoi(argv[i + 2]);
      printf("arg_gcd %i %i, ", arg_gcd_1, arg_gcd_2);
      i += 2;
      got_arg = 1;
    }    
    
    if (((strcmp(argv[i], "-pt")) == 0) || 
        ((strcmp(argv[i], "--print-trigram-freq")) == 0))
    {
      arg_pt = 1;
      printf("arg_pt, ");
      got_arg = 1;
    }

    if (!got_arg)
    {
      printf("arg_bad\n");
      return -1;
    }
    
  }
  
  printf("done.\n");
  return 0;
}
