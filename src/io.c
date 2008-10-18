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

char **cfsh_buffers;
int *cfsh_buffer_filters;
int *cfsh_buffer_sizes;
int cfsh_num_buffers;

void buffers_init()
{
  /* Simple Initialise. Should only be called once, at startup. 
   * Not called by any shell command */

  cfsh_buffers        = NULL;
  cfsh_buffer_filters = NULL;
  cfsh_buffer_sizes   = NULL;
  cfsh_num_buffers    = 0;
}

void create_buffers(int num)
{
  int i, j;

  /* Should never happen */
  if (num < 0)    return -1;

  if (cfsh_num_buffers == 0)
  {
    if (num != 0) 
    {
      printf("create_buffers: Creating buffers for the first time!\n");
      size_buffer_array(num);

      printf("create_buffers: initting all buffers with %i default length\n",
                                    num, DEFAULT_BUFFER_SIZE);
      for (i = 0; i < num; i++)                 initbuffer(i);
    }
    else
    {
      printf("create_buffers: no buffers exist; none created, none cleared\n");
    }
  }
  else if (cfsh_num_buffers > num)
  {
    printf("create_buffers: buffers %i to %i will be discarded\n");
    for (i = num; i < cfsh_num_buffers; i++)    destroybuffer(i);

    printf("create_buffers: reducing array size by %i\n", 
                                     cfsh_num_buffers - num);
    size_buffer_array(num);
  }
  else 
  {
    j = cfsh_num_buffers;  /* We must find the value now as it will change */

    printf("create_buffers: expanding array size by %i\n",
                                     num - cfsh_num_buffers);
    size_buffer_array(num);

    printf("create_buffers: initting %i new buffers with %i default length\n",
                                     num - j, DEFAULT_BUFFER_SIZE);
    for (i = j; i < num; i++)                   initbuffer(i);
  }
}

void size_buffer_array(int num)
{
  cfsh_buffers        = realloc_good( sizeof(char *) * num );
  cfsh_buffer_filters = realloc_good( sizeof(int)    * num );
  cfsh_buffer_sizes   = realloc_good( sizeof(int)    * num );
  cfsh_num_buffers    = num;
}

void file2buffer(char *name, int buffer_id)
{
  int i;
  char *buf;
  FILE *file;

  clearbuffer(buffer_id);

  file = fopen(name, "r");
  if (file == NULL)
  {
    printf("file2buffer: fopen failed: %s\n", strerror(errno));
    return;
  }

  flock(fileno(file), LOCK_SH);

  i = 0;
  while (feof(file) == 0) { i++; fgetc(file) }
  rewind(file);

  if (i > get_buffer_size(buffer_id))
  {
    printf("file2buffer: expanding buffer %i to accomodate infiles %i bytes.\n",
                    buffer_id, i);
    resizebuffer(buffer_id, i);
  }

  i = 0;
  buf = get_buffer(buffer_id);

  while (feof(file) == 0)
  {
    *(buf + i) = fgetc(file);
    i++;
  }

  *(buf + i) = 0;
  setbuffernull(buffer_id);

  printf("file2buffer: loaded %i bytes into buffer %i\n", i, buffer_id);

  flock(fileno(file), LOCK_UN);
  fclose(file);
}

void buffer2file(char *name, int buffer_id, int mode)
{
  int i;
  struct stat statbuf;
  FILE *file;

  i = stat(name, &statbuf);
  if (i == -1)  i = errno;

  file = NULL;

  if (i != ENOENT && i != 0)
  {
    printf("buffer2file: io file error: %s\n", strerror(i));
    return;
  }

  if (i == ENOENT)                           file = fopen(name, "w");
  else if (i == 0 && CFSH_IO_MODE_OVERWRITE) file = fopen(name, "w");
  else if (i == 0 && CFSH_IO_MODE_APPEND)    file = fopen(name, "a");
  else
  {
    printf("buffer2file: File exists!\n");
    return;
  }

  if (file == NULL)
  {
    printf("buffer2file: failed to open file: %s\n", strerror(errno));
    return;
  }

  flock(fileno(file), LOCK_EX);

  setbuffernull(buffer_id);
  i = fputs(get_buffer(buffer_id), file);

  if (i == EOF)       printf("buffer2file: error in fputs.\n");
  if (ferror(file))   printf("buffer2file: warning - outfile error is set.\n");

  flock(fileno(file), LOCK_UN);
  fclose(file);
}

void initbuffer(int buffer_id)
{
  get_buffer(buffer_id)        = malloc_good( DEFAULT_BUFFER_SIZE + 1 );
  get_buffer_size(buffer_id)   = DEFAULT_BUFFER_SIZE;
  get_buffer_filter(buffer_id) = BUFFER_FILTER_NONE;

  clearbuffer(buffer_id);
  setbuffernull(buffer_id);
}

void destroybuffer(int buffer_id)
{
  free(get_buffer(buffer_id));
  get_buffer_size(buffer_id)   = 0;
  get_buffer_filter(buffer_id) = BUFFER_FILTER_NONE;
}

void resizebuffer(int buffer_id, int newsize)
{
  get_buffer(buffer_id) = realloc_good( get_buffer(buffer_id), newsize + 1 );
  get_buffer_size(buffer_id) = newsize;
  get_buffer_filter(buffer_id) = 0;

  clearbuffer(buffer_id);
  setbuffernull(buffer_id);
}

void copybuffer(int buffer_id_1, int buffer_id_2)
{
  printf("copybuffer: copying %i to %i...\n", buffer_id_1, buffer_id_2);

  if (get_buffer_size(buffer_id_2) < get_buffer_size(buffer_id_1))
  {
    printf("copybuffer: must resise buffer %i to %i bytes\n", 
                            buffer_id_2, get_buffer_size(buffer_id_1));
    resizebuffer(buffer_id_2, get_buffer_size(buffer_id_1));
  }
  else if (get_buffer_size(buffer_id_1) == get_buffer_size(buffer_id_2))
  {
    printf("copybuffer: note - buffers arn't the same size (not a problem)\n");
  }

  memcpy(get_buffer(buffer_id_2), get_buffer(buffer_id_1),
                            get_buffer_size(buffer_id_2));
  setbuffernull(buffer_id_2);
}

void filterbuffer(int buffer_id, int mode)
{
  // #define BUFFER_FILTER_NONE       4    /* No filter applied yet */
  // #define BUFFER_FILTER_ALPHA      5    /* Alpha only, case preserved */
  // #define BUFFER_FILTER_ALPHANUM   6    /* Alphanumeric only */
  // #define BUFFER_FILTER_LALPHA     7    /* Alpha only, All lower */
  // #define BUFFER_FILTER_UALPHA     8    /* Alpha only, All upper */
  // #define BUFFER_FILTER_FLIPCASE   9    /* Flip Case */
  // #define BUFFER_FILTER_CASEBACON  10   /* Changes case to ab */
  // #define BUFFER_FILTER_NUM        11   /* Numbers only */
  // #define BUFFER_FILTER_ESP        12   /* All but spaces & xsp chars */
  // #define BUFFER_FILTER_ENL        13   /* All but xsp chars (see macros) */

  int i, j, t, newsize, newpos;
  char ch;
  char *buf;

  newsize = 0;
  buf = get_buffer(buffer_id);
  j = get_buffer_real_size(buffer_id);

  for (i = 0; i < j; i++)
  {
    ch = *(buf + i);

    switch (mode)
    {
      case BUFFER_FILTER_ALPHA:     t = ALPHA_CH(ch);                     break;
      case BUFFER_FILTER_ALPHANUM:  t = ALPHANUMERIC_CH(ch);              break;
      case BUFFER_FILTER_LALPHA:    t = ALPHA_CH(ch);                     break;
      case BUFFER_FILTER_UALPHA:    t = ALPHA_CH(ch);                     break;
      case BUFFER_FILTER_FLIPCASE:  t = ALPHA_CH(ch);                     break;
      case BUFFER_FILTER_CASEBACON: t = ALPHA_CH(ch);                     break;
      case BUFFER_FILTER_NUM:       t = NUMBER_CH(ch);                    break;
      case BUFFER_FILTER_ESP:       t = !(SPACE_CH(ch) || XSPACE_CH(ch)); break;
      case BUFFER_FILTER_ENL:       t = !XSPACE_CH(ch);                   break;
    }

    if (t)  newsize++;
  }

  newpos = 0;

  for (i = 0; i < j; i++)
  {
    ch = *(buf + i);

    switch (mode)
    {
      case BUFFER_FILTER_ALPHA:     t = ALPHA_CH(ch);                     break;
      case BUFFER_FILTER_ALPHANUM:  t = ALPHANUMERIC_CH(ch);              break;
      case BUFFER_FILTER_LALPHA:    t = ALPHA_CH(ch); 
                                    ch = ALPHA_TOLOWER(ch);               break;
      case BUFFER_FILTER_UALPHA:    t = ALPHA_CH(ch);
                                    ch = ALPHA_TOUPPER(ch);               break;
      case BUFFER_FILTER_FLIPCASE:  t = ALPHA_CH(ch);
                                    ch = ALPHA_FLIP_CASE(ch);             break;
      case BUFFER_FILTER_CASEBACON: t = ALPHA_CH(ch);
                                    ch = ALPHA_CASEBACON(ch);             break;
      case BUFFER_FILTER_NUM:       t = NUMBER_CH(ch);                    break;
      case BUFFER_FILTER_ESP:       t = !(SPACE_CH(ch) || XSPACE_CH(ch)); break;
      case BUFFER_FILTER_ENL:       t = !XSPACE_CH(ch);                   break;
    }

    if (t)
    {
      *(buf + newpos) = ch;
      newpos++;
    }
  }

  *(buf + newpos) = 0;
  setbuffernull(buffer_id);

  t = mode;
  switch (mode)
  {
    case BUFFER_FILTER_LALPHA:    t = BUFFER_FILTER_ALPHA;              break;
    case BUFFER_FILTER_UALPHA:    t = BUFFER_FILTER_ALPHA;              break;
    case BUFFER_FILTER_FLIPCASE:  t = BUFFER_FILTER_ALPHA;              break;
  }

  get_buffer_filter(buffer_id) = t;
}

void clearbuffer(int buffer_id)
{
  memset(get_buffer(buffer_id), 0, get_buffer_size(buffer_id));
  setbuffernull(buffer_id);
}

