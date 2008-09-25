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
int arg_ctrans  = 0;         /* Do some sort of columnar transposition */
columnar_transposition_function arg_ctrans_type;
int arg_ctrans_mode = 0;     /* bruteforce, de/en code, flip+de/en code */
int *arg_ctrans_key;         /* If decode, this points to a key array */
int arg_ctrans_key_size = 0;
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
int arg_is_1    = 0;
int arg_bacon   = 0;         /* Do bacon cipher decrypt */
int arg_pb      = 0;         /* Do polybius square decrypt */

int arg_parse(int argc, char **argv)
{
  int i, j, h;
  
  printf("Parsing command line arguments... ");
  
  for (i = 1; i < argc; i++)
  {
    /* Do frequency analysis */
    if (((strcmp(argv[i], "-f")) == 0) || ((strcmp(argv[i], "--freq") == 0)))
    {
      arg_freq = 1;
      printf("arg_freq, ");

    }
    
    /* Input file */
    else if (((strcmp(argv[i], "-i")) == 0) || 
             ((strcmp(argv[i], "--input") == 0)))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad (not enough args)\n");
        return -1;
      }

      printf("arg_input %s, ", argv[i+1]);
      arg_input = argv[i + 1];
      i++; /* We skip the next argument; it is a filename */

    }
    
    /* Shift input */
    else if (((strcmp(argv[i], "-is")) == 0) || 
        ((strcmp(argv[i], "--input-shift") == 0)))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad (not enough args)\n");
        return -1;
      }
      
      printf("arg_is %s, ", argv[i+1]);
      arg_is = 1;
      arg_is_1 = atoi(argv[i+1]);
      i++; /* Skip the next argument, is arg_shift_1 */ 

    }
    
    /* Print a table of mappings from chars to numbers */
    else if (((strcmp(argv[i], "-pct")) == 0) ||
        ((strcmp(argv[i], "--print-char-table") == 0)))
    {
      arg_pct = 1;
      printf("arg_pct, ");

    }
    
    else if ((strcmp(argv[i], "-bac")) == 0 ||
       ((strcmp(argv[i], "--bacon") == 0)))
    {
      arg_bacon = 1;
      printf("arg_bacon, ");

    }
    
    else if ((strcmp(argv[i], "-vig")) == 0 ||
       ((strcmp(argv[i], "--vigenere") == 0)))
    {
      arg_vig = 1;
      printf("arg_vig, ");

    }

    else if ((strcmp(argv[i], "-aff")) == 0 ||
       ((strcmp(argv[i], "--affine") == 0)))
    {
      arg_aff = 1;
      printf("arg_aff, ");

    }
    
    else if ((strcmp(argv[i], "-pb")) == 0 ||
       ((strcmp(argv[i], "--polybius") == 0)))
    {
      arg_pb = 1;
      printf("arg_pb, ");

    }

    else if ((strcmp(argv[i], "-keyb")) == 0 ||
       ((strcmp(argv[i], "--keyword-bruteforce") == 0)))
    {
      arg_keyb = 1;
      printf("arg_keyb, ");

    }

    else if ((strcmp(argv[i], "-keyd")) == 0 ||
       ((strcmp(argv[i], "--keyword-decode") == 0)))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad (not enough args)\n");
        return -1;
      }

      arg_keyd = 1;
      arg_keyd_string = argv[i + 1];
      printf("arg_keyd %s, ", arg_keyd_string);
      i++;

    }

    else if ((strcmp(argv[i], "-ctrans")) == 0 ||
       ((strcmp(argv[i], "--columnar_transposition") == 0)))
    {
      if (i + 2 >= argc)
      {
        printf("arg_bad (not enough args)\n");
        return -1;
      }

      arg_ctrans = 1;


      if ((strcmp(argv[i + 1], "c2c")) == 0 ||
          (strcmp(argv[i + 1], "col2col")) == 0)
      {
        arg_ctrans_type = &columnar_transposition_col2col;
        printf("arg_ctrans col2col: ");
      }
      else if ((strcmp(argv[i + 1], "r2c")) == 0 ||
               (strcmp(argv[i + 1], "row2col")) == 0)
      {
        arg_ctrans_type = &columnar_transposition_row2col;
        printf("arg_ctrans row2col: ");
      }
      else if ((strcmp(argv[i + 1], "c2r")) == 0 ||
               (strcmp(argv[i + 1], "col2row")) == 0)
      {
        arg_ctrans_type = &columnar_transposition_col2row;
        printf("arg_ctrans col2row: ");
      }
      else
      {
        printf("arg_bad (must be c2c, r2c or c2r)\n");
        return -1;
      }

      if ((strcmp(argv[i + 2], "b")) == 0 ||
          (strcmp(argv[i + 2], "bruteforce")) == 0)
      {
        arg_ctrans_mode = 0;
        printf("bruteforce mode, ");
      }
      else if ((strcmp(argv[i + 2], "d")) == 0 ||
               (strcmp(argv[i + 2], "decode")) == 0)
      {
        arg_ctrans_mode = 1;
        printf("decode mode: ");
      }
      else if ((strcmp(argv[i + 2], "f")) == 0 ||
               (strcmp(argv[i + 2], "flipdecode")) == 0)
      {
        arg_ctrans_mode = 2;
        printf("flipdecode mode: ");
      }
      else
      {
        printf("arg_bad (must be b, d or f)\n");
        return -1;
      }

      if (arg_ctrans_mode == 1 || arg_ctrans_mode == 2)
      {
        if (i + 3 >= argc)
        {
          printf("arg_bad (not enough args)\n");
          return -1;
        }

        arg_ctrans_key_size = atoi(argv[i + 3]);

        if (arg_ctrans_key_size < 2 || i + 3 + arg_ctrans_key_size >= argc)
        {
          printf("arg_bad (bad key size or not enough elements)\n");
          return -1;
        }

        arg_ctrans_key = malloc( sizeof(int) * arg_ctrans_key_size );

        if (arg_ctrans_key == NULL)
        {
          printf("\n Failed to allocate (sizeof_int:%i * key_size:%i) = %i\n",
          sizeof(int), arg_ctrans_key_size, sizeof(int) * arg_ctrans_key_size);
          exit(1);
        }

        for (j = 0; j < arg_ctrans_key_size; j++)
        {
          arg_ctrans_key[j] = atoi(argv[i + 4 + j]);

          if (arg_ctrans_key[j] < 0 || arg_ctrans_key[j] >= arg_ctrans_key_size)
          {
            printf("arg_bad (bad key element)\n");
            return -1;
          }

          for (h = 0; h < j; h++)
          {
            if (arg_ctrans_key[h] == arg_ctrans_key[j])
            {
              printf("arg_bad (duplicate key element)\n");
              return -1;
            }
          }
        }

        i += 3 + arg_ctrans_key_size;
      }
      else
      {
        i += 2;
      }
    }

    else if ((strcmp(argv[i], "-gcd")) == 0)
    {
      if (i + 2 >= argc)
      {
        printf("arg_bad (not enough args)\n");
        return -1;
      }

      arg_gcd = 1;
      arg_gcd_1 = atoi(argv[i + 1]);
      arg_gcd_2 = atoi(argv[i + 2]);
      printf("arg_gcd %i %i, ", arg_gcd_1, arg_gcd_2);
      i += 2;

    }    

    else if ((strcmp(argv[i], "-mmi")) == 0)
    {
      if (i + 2 >= argc)
      {
        printf("arg_bad (not enough args)\n");
        return -1;
      }

      arg_mmi = 1;
      arg_mmi_1 = atoi(argv[i + 1]);
      arg_mmi_2 = atoi(argv[i + 2]);
      printf("arg_mmi %i %i, ", arg_mmi_1, arg_mmi_2);
      i += 2;

    }
 
   
    else if (((strcmp(argv[i], "-pt")) == 0) || 
        ((strcmp(argv[i], "--print-trigram-freq")) == 0))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad (not enough args)\n");
        return -1;
      }

      arg_pt = 1;
      arg_pt_1 = atoi(argv[i + 1]);
      printf("arg_pt %d, ", arg_pt_1);
      i++;

    }

    else if (((strcmp(argv[i], "-pd")) == 0) ||
        ((strcmp(argv[i], "--print-digram-freq")) == 0))
    {
      if (i + 1 >= argc)
      {
        printf("arg_bad (not enough args)\n");
        return -1;
      }

      arg_pd = 1;
      arg_pd_1 = atoi(argv[i + 1]);
      printf("arg_pd %d, ", arg_pd_1);
      i++;

    }
    
    else if (((strcmp(argv[i], "-fd")) == 0) || 
        ((strcmp(argv[i], "--frequency-guesses")) == 0))
    {
      arg_fd = 1;
      printf("arg_fd, ");

    }
    else
    {
      printf("arg_bad (not got)\n");
      return -1;
    }
    
  }
  
  printf("done.\n");
  return 0;
}
