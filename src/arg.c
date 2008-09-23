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

char *arg_input = NULL;    /* !NULL = load this file as input */
int arg_freq    = 0;         /* Do frequency analysis */
int arg_pct     = 0;         /* Print char->number table */
int arg_vig     = 0;         /* Do a vigenere crack */
int arg_aff     = 0;         /* Do an affine crack */
int arg_keyb    = 0;         /* Do a Keyword bruteforce */
int arg_keyd    = 0;         /* Do a keyword decode */
char *arg_keyd_string = NULL; 
int arg_gcd     = 0;         /* Do a GCD Calculation */
int arg_gcd_1   = 0;
int arg_gcd_2   = 0;
int arg_mmi     = 0;         /* Do a ModMulInv Calculation */
int arg_mmi_1   = 0;
int arg_mmi_2   = 0;
int arg_pt      = 0;         /* Print trigram frequencies */
int arg_pt_1    = 0;
int arg_pd      = 0;         /* Print digram frequencies */
int arg_pd_1    = 0;
int arg_fd      = 0;         /* Frequency guesses */
int arg_is      = 0;         /* Shift the intext by arg_shift_1 */
int arg_is_1   = 0;

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
    
    /* Shift input */
    if (((strcmp(argv[i], "-is")) == 0) || ((strcmp(argv[i], "--input-shift") == 0)))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad\n");
        return -1;
      }
      
      printf("arg_is %s, ", argv[i+1]);
      arg_is = 1;
      arg_is_1 = atoi(argv[i+1]);
      i++; /* Skip the next argument, is arg_shift_1 */ 
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
      printf("arg_aff, ");
      got_arg = 1;
    }

    if ((strcmp(argv[i], "-keyb")) == 0 ||
       ((strcmp(argv[i], "--keyword-bruteforce") == 0)))
    {
      arg_keyb = 1;
      printf("arg_keyb, ");
      got_arg = 1;
    }

    if ((strcmp(argv[i], "-keyd")) == 0 ||
       ((strcmp(argv[i], "--keyword-decode") == 0)))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad\n");
        return -1;
      }

      arg_keyd = 1;
      arg_keyd_string = argv[i + 1];
      printf("arg_keyd %s, ", arg_keyd_string);
      i++;
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

    if ((strcmp(argv[i], "-mmi")) == 0)
    {
      if (i + 2 >= argc)
      {
        printf("arg_bad\n");
        return -1;
      }

      arg_mmi = 1;
      arg_mmi_1 = atoi(argv[i + 1]);
      arg_mmi_2 = atoi(argv[i + 2]);
      printf("arg_mmi %i %i, ", arg_mmi_1, arg_mmi_2);
      i += 2;
      got_arg = 1;
    }
 
   
    if (((strcmp(argv[i], "-pt")) == 0) || 
        ((strcmp(argv[i], "--print-trigram-freq")) == 0))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad\n");
        return -1;
      }

      arg_pt = 1;
      arg_pt_1 = atoi(argv[i + 1]);
      printf("arg_pt %d, ", arg_pt_1);
      i++;
      got_arg = 1;
    }

    if (((strcmp(argv[i], "-pd")) == 0) ||
        ((strcmp(argv[i], "--print-digram-freq")) == 0))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad\n");
        return -1;
      }

      arg_pd = 1;
      arg_pd_1 = atoi(argv[i + 1]);
      printf("arg_pd %d, ", arg_pd_1);
      i++;
      got_arg = 1;
    }
    
    if (((strcmp(argv[i], "-fd")) == 0) || 
        ((strcmp(argv[i], "--frequency-guesses")) == 0))
    {
      arg_fd = 1;
      printf("arg_fd, ");
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
