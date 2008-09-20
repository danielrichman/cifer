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

int *ch_freq_ar;

int main(int argc, char **argv)
{
  if ((arg_parse(argc, argv)) == -1) /* Bad argument(s) */
  {
    printf("Bad argument(s), exiting!\n");
    return 0;
  }
  
  if (arg_input) /* Have we got any input files? */
  {
    input_open(arg_input_argc, argv);
  }

  /* TODO: access action.c now to take over. */

  /* Temporary: some diagnostics & cracking info */

  load_dict();
  crack_vigenere(intext, intext_size);
  printf("\n\n%*s\n\n", intext_size, intext);

  return 0;
} 

