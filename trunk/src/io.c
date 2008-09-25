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

char *intext, *intext_num, *intext_original, *intext_num_original;
int intext_size, intext_num_size, intext_original_size,
    intext_num_original_size;

int input_open(char *filename)
{
  FILE *infile;
  int i;
  char ch;
  
  printf("Opening input source(s)... ");
  
  if ((infile = fopen(filename, "r")) == NULL)
  {
    printf("failed, fopen returned NULL!\n");
    return INPUT_ERR_FOPEN;
  }
  printf("opened %s, ", filename);
  
  /* Find out how big the file is */
  if (!(((intext_size = input_file_size_alpha(infile)) == 0) ||
      ((intext_num_size = input_file_size_alphanum(infile)) == 0)))
  {
    printf("failed, intext_size is 0!\n");
    return INPUT_ERR_ZERO_SIZE;
  }
  printf("intext_size %d bytes, \n", intext_size);
   
  if ((intext_num_size = input_file_size_alphanum(infile)) == 0)
  {
    printf("failed, intext_num_size is 0!\n");
    return INPUT_ERR_ZERO_SIZE;
  }
  printf("intext_num_size %d bytes, \n", intext_num_size);

  /* Allocate space */
  intext              = malloc(intext_size     + 1);
  intext_original     = malloc(intext_size     + 1);
  intext_num          = malloc(intext_num_size + 1);
  intext_num_original = malloc(intext_num_size + 1);

  /* malloc() enough space for the file */
  if (intext == NULL || intext_original == NULL || intext_num == NULL ||
      intext_num_original == NULL)
  {
    printf("intext_* failed, malloc(%i) returned NULL!\n", 
                       intext_size + 1);
    return INPUT_ERR_MALLOC;
  }

  /* Makes it look nicer =) */
  printf("                            ");
  printf("malloc()'d %d bytes intext, %d bytes intext_num, ", intext_size + 1,
                                                          intext_num_size + 1);
 
  /* Set the final null byte */
  *(intext     + intext_size    ) = 0;
  *(intext_num + intext_num_size) = 0;
 
  /* Read the file into intext */
  ch = 0;
  
    /* intext is alpha upper/lower only */
  for (i = 0; i < intext_size; ch = fgetc(infile))
  {
    if (ALPHAL_CH(ch) || ALPHAH_CH(ch))
    {
      *(intext + i) = ch;  /* Preserve case */
      i++;
    }
  }
  printf("read into intext, ");
  
  ch = 0;
  rewind(infile);
  
    /* intext_num is alphanumeric only */
  for (i = 0; i < intext_num_size; ch = fgetc(infile))
  {
    if (ALPHAL_CH(ch) || ALPHAH_CH(ch) || NUMBER_CH(ch))
    {
      *(intext_num + i) = ch;  /* Preserve case */
      i++;
    }
  }
  printf("read into intext_num, ");
  printf("done.\n");
 
  /* Backup the ctext */
  memcpy(intext_original,     intext,     intext_size    );
  memcpy(intext_num_original, intext_num, intext_num_size);
  
  intext_original_size     = intext_size;
  intext_num_original_size = intext_num_size;
  
  printf("Copied intext_* to intext_*_original (memcpy)\n");
 
  return 0;
}

void input_flip_case()
{
  int i;

  for (i = 0; i < intext_size; i++)
  {
    *(intext + i) = ALPHA_FLIP_CASE(*(intext + i));
  }
}

void input_restore()
{
  printf("Restored intext from intext_original, size (old/new): %i/%i\n",
              intext_size, intext_original_size);
  intext_size = intext_original_size;
  memcpy(intext, intext_original, intext_original_size);
}

int input_file_size_alpha(FILE *infile)
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

int input_file_size_alphanum(FILE *infile)
{
  int size = 0;
  char ch;

  /* We assume that its already rewound. */

  while (feof(infile) == 0)
  {
    ch = fgetc(infile);

    if (ALPHAL_CH(ch) || ALPHAH_CH(ch) || NUMBER_CH(ch))
    {
      size++;
    }
  }

  rewind(infile);

  return size;
}

void shift_text(char *intext, int intext_size, int shift)
{
  int i;
  
  printf("Shifting text by %d... ", shift);
  
  for (i = 0; i < intext_size; i++)
  {
    *(intext + i) = NUMCHAR(modp(CHARNUM(*(intext + i)) + shift, 26));
  }
  
  printf("done.\n");
}

int print_file(char *tofile, int tofile_size, char *filename, int filename_size,
                char *mode)
{
  FILE *fp;
  int size;
  
  
  printf("Printing output to %s... ", tofile);
  
  if ((fp = fopen(filename, mode)) == NULL)
  {
    printf("failed, fopen() returned NULL!\n");
    return PRINT_FILE_ERR_FOPEN;
  }
  
  if ((size = fprintf(fp, "%s", tofile)) < 0)
  {
    printf("failed, fprintf() error!\n");
    return PRINT_FILE_ERR_FPRINTF;
  }
  
  printf("printed %d/%d byte(s), done\n", size, tofile_size);
  
  return 0;
}
