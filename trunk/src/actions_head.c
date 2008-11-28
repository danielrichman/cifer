/*
    Cifer: Automating classical cipher cracking in C
    Copyright (C) 2008  Daniel Richman & Simrun Basuita

    Cifer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cifer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cifer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdinc.h"

int actionu_intparse_f(char *str, int *value, int char_mode, void *failfree)
{
  size_t sz;
  char *validator, *target;

  sz = strlen(str);

  if (char_mode && strtlens(str, sz) == 1 &&
      ALPHA_CH( *(str + strlefts(str, sz)) ))
  {
    *value = CHARNUM( *(str + strlefts(str, sz)) );
    return 0;
  }

  target = str + sz - strrights(str, sz);
  *value = strtol(str + strlefts(str, sz), &validator, 10);

  if (validator != target)
  {
    if (failfree != NULL)   free(failfree);
    return -1;
  }

  return 0;
}

int actionu_bufferparse_f(char *str)
{
  /* strlen("buffer_") = 7 */
  size_t sz;
  long j;
  char *validator;

  sz = strlen(str);
  if (strtlens(str, sz) < 8)                        return -1;
  if (strncasecmp(str, "buffer_", 7) != 0)          return -1;

  j = strtol(str + strlefts(str, sz) + 7, &validator, 10);

  if (validator != (str + sz - strrights(str, sz))) return -1;
  if (j > cfsh_num_buffers)                         return -1;
  if (j < 0)                                        return -1;

  return j;
}

int actionu_bufferchk_f(int buffer_in, int buffer_out)
{
  if (get_buffer_real_size(buffer_in) == 0)
  {
    printf("bad input buffer: no data.\n");
    return -1;
  }

  if (buffer_in == buffer_out)
  {
    printf("bad output buffer: same as input buffer.\n");
    return -1;
  }

  return 0;
}

int actionu_bufferfchk_f(int buffer_id, int filter)
{
  if (get_buffer_filter(buffer_id) != filter)
  {
    printf("bad input buffer: filter type '%s' should be '%s'. \n",
                     get_buffer_filter_text(b), get_filter_text(f) );
    return -1;
  }

  return 0;
}

int actionu_bufferschk_f(int buffer_id, int buffer_out)
{
  if (get_buffer_real_size(i) > get_buffer_size(o))
  {
    printf("auto expanding output buffer %i to %i bytes.\n", o,
                  get_buffer_real_size(i));
    resizebuffer(o, get_buffer_real_size(i));
  }
}

int actionu_copysize(int buffer_in, int buffer_out)
{
  *(get_buffer(o) + get_buffer_real_size(i)) = 0;
  setbuffernull(o);
}

