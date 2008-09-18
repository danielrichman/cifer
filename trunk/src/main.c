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
    printf("Bad arguments, exiting!\n");
    /* Show usage info */
    return 0;
  }
    
  cc_setup(argc, argv);
  ch_freq_ar = ch_freq(intext, intext_size);
  print_freq(ch_freq_ar, 26);
  
  crack_vigenere(intext, intext_size);

  return 0;
} 

