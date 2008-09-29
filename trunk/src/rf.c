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

void rf_bf(char *intext, int intext_size, int maxrails)
{
  int /*i,*/ j;
  int currail;
  int currailbuf_count = 0;
  char currailbuf[intext_size];
  //char otext[intext_size];
  
  printf("\n\n");
  
  for (currail = 1; currail < maxrails; currail++)
  {
    for (j = 0; j < intext_size;)
    {
      currailbuf[currailbuf_count] = *(intext + j);
      currailbuf_count++;
      
      printf("%c", currailbuf[currailbuf_count]);
      
      if ((j + 2*currail) > intext_size) break;
      j += 2*currail;
    }
    
    currailbuf_count = 0;
    printf("\n\n");
  }
  
  printf("\n\n");
}
