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

int arg_input = 0; /* Do we have any input files? */
int arg_freq  = 0; /* Do frequency analysis */
int arg_pct   = 0; /* Print char->number table */

int arg_parse(int argc, char **argv)
{
  int i;
  int got_arg = 0;
  
  printf("Parsing command line arguments... ");
  
  for (i = 1; i < argc; i++)
  {
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
      printf("arg_input %s, ", argv[i+1]);
      /* The next argument should be a filename, pass it to arg_file_open() */
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
    
    if (!got_arg)
    {
      printf("arg_bad\n");
      return -1;
    }
    
  }
  
  printf("done.\n");
  return 0;
}
