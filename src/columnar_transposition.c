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

/* KEY FORMAT: int array of size "key size", no recurring numbers
 * to use, the columns are shifted. ZEBRAS: 521304. The key allways
 * encodes; to decode you have to flip the key first =) (there's a 
 * function below to do that). The key must be starting at 0. */

void columnar_transposition_bruteforce(char *text, int text_size,
          int key_min, int key_max, columnar_transposition_function *routine)
{
  int key_size, i, j, k, t, it, score, best_score, best_size;
  int *key, *key_best;
  char *text_tmp;

  /* Allocate to the maximum size, that's means we only have to alloc once */
  key = malloc(sizeof(int) * key_max);
  key_best = malloc(sizeof(int) * key_max);
  if (key == NULL)
  {
    printf("c.trans._bruteforce malloc(%i * %i) key/key_best = fail\n",
                                                sizeof(int), key_max);
    exit(1);
  }

  /* Create our temporary space */
  text_tmp = malloc(text_size);
  if (text_tmp == NULL)
  {
    printf("c.trans._bruteforce malloc(%i) text_temp fail\n", text_size);
    exit(1);
  }

  /* Prepare */
  best_score = -1;

  /* Start teh cracking! */
  for (key_size = key_min; key_size < key_max; key_size++)
  {
    /* Calcualte the factorial */
    k = 1;
    for (i = 1; i <= key_size; i++)  k *= i;

    /* Initialise the key */
    for (i = 0; i < key_size; i++)  key[i] = i;

    /* Crack */
    for (i = 0; i < k; i++)
    {
      /* Generate the permutation */
      it = i;
      for (j = 1; j < key_size; j++)
      {
        it = it / j;
        t = key[j - 1];
        key[j - 1] = key[modp(it, j)];
        key[modp(it, j)] = t;
      }

      /* Try it */
      (*routine)(text, text_tmp, text_size, key, key_size);

      /* Score it */
      score = score_text_dict_fast(text_tmp, text_size);

      if (score > best_score)
      {
        best_score = score;
        best_size = key_size;
        for (j = 0; j < key_size; j++) key_best[j] = key[j];
      }
    }
  }

  /* Results */
  (*routine)(text, text_tmp, text_size, key_best, best_size);

  printf("Columnar Transposition Bruteforce: ");
  for (i = 0; i < best_size; i++) printf("%i|", key_best[i]);
  printf(" (%i) \n\n", best_score);
  printf("%*s\n\n", text_size, text);

  /* Free up */
  free(key);
  free(key_best);
  free(text_tmp);
}

/* Reads off into columns KEY_SIZE, reorders as per key, reads off
 * in columns - the key is array[source] => target */
/* THE INPUT TEXT AND THE OUTPUT TEXT MUST BE SEPARATE STRINGS */
void columnar_transposition_col2col(char *text, char *outtext, 
                           int text_size, int *key, int key_size)
{
  int i, chunk_start, key_item, target;

  chunk_start = 0;
  for (i = 0; i < text_size; i++)
  {
    key_item = modp(i, key_size);
    if (key_item == 0)  chunk_start = i;
    if (chunk_start + key_size >= text_size) break;
    target = chunk_start + key[key_item];

    *(outtext + target) = *(text + i);
  }

  /* Any remainter should just be copied from a => b */
  while (i < text_size)
  {
    *(outtext + i) = *(text + i);
    i++;
  }
}

/* Essentially writes out the text in cols (top to bottom then 
 * reads off in rows (left to right). This  is a DECODER,
 * however it requires a flipped key. */
void columnar_transposition_col2row(char *text, char *outtext, 
                           int text_size, int *key, int key_size)
{
  int i, col_start, col_end, col_num, col_std_length, cols_incomplete, target;

  col_start = 0; col_end = 0; col_num = -1;
  cols_incomplete = modp(text_size, key_size);
  col_std_length = (text_size - cols_incomplete) / key_size;

  for (i = 0; i < text_size; i++)
  {
    if (i == col_end)
    {
      /* New col calculation is needed */
      col_num++;
      col_start = i;
      col_end = i + col_std_length + (key[col_num] < cols_incomplete ? 1 : 0);
    }

    target = key[col_num] + ((i - col_start) * key_size);
    *(outtext + target) = *(text + i);
  }
}

/* Encodes, writes out in rows (l => r) and reads off in columns. */
void columnar_transposition_row2col(char *text, char *outtext, 
                           int text_size, int *key, int key_size)
{
  int i, cols_incomplete, col_std_length, col_num, col_start, col_pos;

  cols_incomplete = modp(text_size, key_size);
  col_std_length = (text_size - cols_incomplete) / key_size;

  for (col_num = 0; col_num < key_size; col_num++)
  {
    col_start = col_std_length * key[col_num];
    col_pos = 0;

    for (i = 0; i < key_size; i++)
      if (i < cols_incomplete && key[i] < key[col_num]) col_start++;

    for (i = col_num; i < text_size; i += key_size, col_pos++)
    {
      /* Source = i, target = col_start + col_pos */
      *(outtext + col_start + col_pos) = *(text + i);
    } 
  }
}

void columnar_transposition_flip_key(int *key, int key_size)
{
  int i, *temp;  /* Dynamic size */

  temp = malloc( sizeof(int) * key_size);
  if (temp == NULL)
  {
    printf("columnar_transposition_flip_key: malloc(%i) temp failed.\n",
                                  key_size);
    exit(1);
  }

  /* copy into temp, then flip */
  for (i = 0; i < key_size; i++) temp[i] = key[i];
  for (i = 0; i < key_size; i++) key[temp[i]] = i;

  /* Free up */
  free(temp);
}
