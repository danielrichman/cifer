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

char *intext;
int intext_size;

int input_open(int argc, char **argv)
{
  FILE *infile;
  int i;
  char ch;
  
  printf("Opening input source(s)... ");
  
  if ((infile = fopen(argv[argc], "r")) == NULL)
  {
    printf("failed, fopen returned NULL!\n");
    return INPUT_ERR_FOPEN;
  }
  printf("opened %s, ", argv[argc]);
  
  /* Find out how big the file is */
  if ((intext_size = input_file_size(infile)) == 0)
  {
    printf("failed, intext_size is 0!\n");
    return INPUT_ERR_ZERO_SIZE;
  }
  printf("intext_size %d bytes, \n", intext_size);
  printf("                            ");

  /* malloc() enough space for the file */
  if ((intext = malloc(intext_size + 1)) == NULL)
  {
    printf("failed, malloc(%i) returned NULL!\n", intext_size + 1);
    return INPUT_ERR_MALLOC;
  }
  printf("malloc()'d %d bytes, ", intext_size + 1);
 
  /* Set the final null byte */
  *(intext + intext_size) = 0;
 
  /* Read the file into intext */
  for (i = 0; i < intext_size; ch = fgetc(infile))
  {
    if (ALPHAL_CH(ch) || ALPHAH_CH(ch))
    {
      *(intext + i) = ALPHA_TOLOWER(ch);
      i++;
    }
  }
  printf("read into intext, ");
  printf("done.\n");
  
  return 0;
}

int input_file_size(FILE *infile)
{
  int size = 0;
  char ch;
  
  /* We assume that its already rewound. */

  while (feof(infile) == 0)
  {
    ch = fgetc(infile);
    
    if (ALPHAL_CH(ch) || ALPHAH_CH(ch))
    {
      size++;
    }
  }

  rewind(infile);  

  return size;
}
