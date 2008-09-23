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

    target = col_num + ((i - col_start) * key_size);
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
    col_start = col_std_length + key[col_num];
    col_pos = 0;
    if (col_num > cols_incomplete)  col_start += cols_incomplete;
    else                            col_start += col_num;

    for (i = key[col_num]; i < text_size; i += key_size, col_pos++)
    {
      /* Source = i, target = col_start + col_pos */
      *(outtext + col_start + col_pos) = *(text + i);
      col_pos++;
    } 
  }
}

void columnar_transposition_flip_key(int *key, int key_size)
{
  int i, *temp;  /* Dynamic size */

  temp = malloc(key_size);
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

