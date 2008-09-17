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

void cc_setup(int argc, char **argv)
{
  FILE *infile;
  int i;
  char ch;

  printf("\n");

  if (argc != 2)
  {
    printf("Usage: cipher_cracker input_filename\n\n");
    exit(1);
  }

  infile  = fopen(argv[1], "r");
  if (infile == NULL)
  {
    perror("infile-fopen");
    printf("\n");
    exit(1);
  }

  /* Load up the file's contents. */

  intext_size = 0;
  while (feof(infile) == 0)
  {
    ch = fgetc(infile);

    if (ALPHAL_CH(ch) || ALPHAH_CH(ch))
    {
      intext_size++;
    }
  }

  if (intext_size == 0)
  {
    printf("Input file is of 0 length.\n\n");
    exit(1);
  }

  rewind(infile);
  intext     = malloc( intext_size + 1 );

  if (intext == NULL)
  {
    printf("*intext malloc( %i ) fail.\n\n", intext_size);
    exit(1);
  }

  printf("Loading %i bytes of infile... ", intext_size);
  fflush(stdout);

  for (i = 0; i < intext_size; ch = fgetc(infile))
  {
    if (ALPHAL_CH(ch) || ALPHAH_CH(ch))
    {
      *(intext + i) = ALPHA_TOLOWER(ch);
      i++;
    }
  }

  printf("Done\n\n");
  fflush(stdout);

  load_dict();
}

void print_freq(int *freq, int size)
{
  int i;
  int j = 0; 
  
  for (i = 0; i < size; i++)
  {
    printf("%c:%d ", NUMCHAR(i), *(freq + i));
    j++;
    if (j == 5)
    {
      printf("\n");
      j = 0;
    }
  }
  printf("\n");
}
