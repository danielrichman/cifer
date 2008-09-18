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

int arg_freq = 0;

int arg_parse(int argc, char **argv)
{
  int i;
  
  printf("Parsing command line arguments... ");
  
  for (i = 0; i < argc; i++)
  {
    /* Do frequency analysis */
    if (((strcmp(argv[i], "-f")) == 0) || ((strcmp(argv[i], "--freq") == 0)))
    {
      arg_freq = 1;
      printf("arg_freq, ");
      break;
    }
    
    /* Input file */
    if (((strcmp(argv[i], "-i")) == 0) || ((strcmp(argv[i], "--input") == 0)))
    {
      printf("arg_input, ");
      /* The next argument should be a filename, pass it to arg_file_open() */
      /* arg_file_open(argv[i+1]); */
      i++; /* We skip the next argument; it is a filename */
      break;
    }
    
    /* If we get to here, it's a bad argument. Return -1. */
    printf("arg_bad\n");
    return -1;
    
  }
  
  printf("done.\n");
  return 0;
}
