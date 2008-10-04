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

int main(int argc, char **argv)
{
  if ((arg_parse(argc, argv)) == -1) /* Bad argument(s) */
  {
    printf("Bad argument(s), exiting!\n");
    return 0;
  }
  
  action_do();
  
  int i;
  int spaces[8][2];
  rf_get_spaces(8, spaces);
  for (i = 0; i < 8; i++)
  {
    printf("%d rails, spaces1 = %d, spaces2 = %d\n", i, spaces[i][0],
                                                        spaces[i][1]);
  }
  //rf_bf(NULL, 0, 0, 10);
  
  return 0;
} 
