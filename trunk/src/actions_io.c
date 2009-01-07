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

/* TODO: FIX!!
 * action_wordwrap fails (libc bails) if settings.h:DEFAULT_BUFFER_SIZE is
 * low which it should be;
 * *** glibc detected *** ./cifer: double free or corruption (out): 0x08f11138 ***
 * ======= Backtrace: =========
 * /lib/tls/i686/cmov/libc.so.6[0xb7e133f4]
 * /lib/tls/i686/cmov/libc.so.6(cfree+0x96)[0xb7e15456]
 * /lib/libreadline.so.5(rl_set_prompt+0x31)[0xb7f102c1]
 * /lib/libreadline.so.5(readline+0x29)[0xb7f10359]
 * ./cifer[0x805fad6]
 * ./cifer[0x805fcc3]
 * ./cifer[0x805ed42]
 * /lib/tls/i686/cmov/libc.so.6(__libc_start_main+0xe5)[0xb7dba685]
 * ./cifer[0x8048d81] */

#include "stdinc.h"

#define ACTION_USAGE action_buffers_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_buffers(int argc, char **argv)
{
  int num;

  actionu_argchk(1)
  actionu_intparse(*(argv), num)

  if (num < 0) actionu_fail()

  create_buffers(num);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_resize_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_resize(int argc, char **argv)
{
  int newsize, buffer_id;

  actionu_argchk(2)
  actionu_intparse(*(argv + 1), newsize)
  actionu_bufferparse(*(argv), buffer_id)

  if (newsize < 1) actionu_fail()

  printf("resizebuffer: resizing buffer_%i to %i bytes...\n", 
                                      buffer_id, newsize);
  resizebuffer(buffer_id, newsize);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_clear_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_clear(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1)
  actionu_bufferparse(*(argv), buffer_id)
  printf("clearbuffer: clearing buffer_%i...\n", buffer_id);
  clearbuffer(buffer_id);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_copy_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_copy(int argc, char **argv)
{
  int buffer_id_1, buffer_id_2;
  actionu_argchk(2)
  actionu_bufferparse(*(argv),     buffer_id_1)
  actionu_bufferparse(*(argv + 1), buffer_id_2)
  actionu_bufferchk(buffer_id_1, buffer_id_2)
  copybuffer(buffer_id_1, buffer_id_2);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_load_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_load(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(2)
  actionu_bufferparse(*(argv + 1), buffer_id)
  file2buffer(*(argv), buffer_id);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_write_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_write(int argc, char **argv)
{
  int mode, buffer_id;

  if (argc != 2 && argc != 3) actionu_fail()

  if (argc == 3)
  {
    if (strcasecmp(*(argv + 2), "auto")      == 0)
    {
      mode = CFSH_IO_MODE_AUTO;
    }
    else if (strcasecmp(*(argv + 2), "overwrite") == 0)
    {
      mode = CFSH_IO_MODE_OVERWRITE;
    }
    else if (strcasecmp(*(argv + 2), "append")    == 0)
    {
      mode = CFSH_IO_MODE_APPEND;
    }
    else
    {
      printf(action_write_usage);
      return CFSH_COMMAND_HARDFAIL;
    }
  }
  else
  {
    mode = CFSH_IO_MODE_AUTO;
  }

  actionu_bufferparse(*(argv + 1), buffer_id)
  buffer2file(*(argv), buffer_id, mode);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_filter_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_filter(int argc, char **argv)
{
  int mode, buffer_id;
  actionu_argchk(2)
  actionu_bufferparse(*(argv), buffer_id)

  mode = get_buffer_filter_fromtext(*(argv + 1));
  if (mode == BUFFER_FILTER_NONE)
  {
    printf("filter: invalid filter name\n");
    actionu_faili()
  }

  filterbuffer(buffer_id, mode);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_read_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_read(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1)
  actionu_bufferparse(*(argv), buffer_id)
  printf("  Buffer %i: %i/%i bytes. Filter %s. \n\n", buffer_id, 
         get_buffer_real_size(buffer_id), get_buffer_size(buffer_id),
         get_buffer_filter_text(buffer_id));
  printf("%s\n", get_buffer(buffer_id));
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_bufferinfo_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_bufferinfo(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1)
  actionu_bufferparse(*(argv), buffer_id)
  printf("  Buffer %i: %i/%i bytes. Filter %s. \n", buffer_id, 
         get_buffer_real_size(buffer_id), get_buffer_size(buffer_id),
         get_buffer_filter_text(buffer_id));
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_nullbuffer_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_nullbuffer(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1)
  actionu_bufferparse(*(argv), buffer_id)
  setbuffernull(buffer_id);
  printf("set buffer %i's safety-null byte.\n", buffer_id);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_script_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_script(int argc, char **argv)
{
  actionu_argchk(1)
  cfsh_scriptfile(*argv, 0, 0);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_quickscript_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_quickscript(int argc, char **argv)
{
  actionu_argchk(1)
  cfsh_scriptfile(*argv, 1, 0);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_softscript_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_softscript(int argc, char **argv)
{
  actionu_argchk(1)
  cfsh_scriptfile(*argv, 0, 1);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_quicksoftscript_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_quicksoftscript(int argc, char **argv)
{
  actionu_argchk(1)
  cfsh_scriptfile(*argv, 1, 1);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_score_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_score(int argc, char **argv)
{
  int buffer_id, sizecache;
  actionu_argchk(1)
  actionu_bufferparse(*(argv), buffer_id)
  sizecache = get_buffer_real_size(buffer_id);
  actionu_dictcheck()

  printf("  Buffer %i: %i/%i bytes. Filter %s. \n", buffer_id,
         sizecache, get_buffer_size(buffer_id),
         get_buffer_filter_text(buffer_id));
  printf("  Dictionary Score: %i/%i \n", 
         score_text_dict_fast(get_buffer(buffer_id), sizecache), sizecache);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_spaces_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_spaces(int argc, char **argv)
{
  int buffer_in, buffer_out, sizecache, sizeadd, i, j;
  int *space_array;
  char *in, *out;

  actionu_argchk(2)
  actionu_bufferparse(*(argv)    , buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferfchk(buffer_in, BUFFER_FILTER_ESP)
  actionu_dictcheck()

  in  = get_buffer(buffer_in);
  out = get_buffer(buffer_out);
  get_buffer_filter(buffer_out) = BUFFER_FILTER_ENL;

  sizecache = get_buffer_real_size(buffer_in);
  sizeadd = 0;

  space_array = malloc_good( sizeof(int) * sizecache );
  score_text_dict_spaces(in, sizecache, space_array);

  for (i = 0; i < sizecache; i++) if (*(space_array + i)) sizeadd++;

  if (get_buffer_size(buffer_out) < (sizecache + sizeadd))
  {
    printf("spaces: must increase buffer size to %i bytes.\n", 
                                           sizecache + sizeadd);
    resizebuffer(buffer_out, sizecache + sizeadd);
  }

  j = 0;
  for (i = 0; i < sizecache; i++)
  {
    *(out + j) = *(in + i);
    j++;

    if (*(space_array + i))
    {
      *(out + j) = ' ';
      j++;
    }
  }

  printf("\n%s\n\n", out);
  free(space_array);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_wordwrap_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_wordwrap(int argc, char **argv)
{
  int buffer_in, buffer_out, sizecache, sizeadd, i, j;
  int *nl_array;
  char *in, *out;

  actionu_argchk(2)
  actionu_bufferparse(*(argv)    , buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferfchk(buffer_in, BUFFER_FILTER_ENL)
  actionu_bufferchk(buffer_in, buffer_out)

  in  = get_buffer(buffer_in);
  out = get_buffer(buffer_out);
  get_buffer_filter(buffer_out) = BUFFER_FILTER_NONE;

  sizecache = get_buffer_real_size(buffer_in);
  sizeadd = 0;

  nl_array = malloc_good( sizeof(int) * sizecache );
  cf_wordwrap(in, sizecache, nl_array);

  for (i = 0; i < sizecache; i++) if (*(nl_array + i)) sizeadd++;

  if (get_buffer_size(buffer_out) < (sizecache + sizeadd))
  {
    printf("wordwrap: must increase buffer size to %i bytes.\n",
                                           sizecache + sizeadd);
    resizebuffer(buffer_out, sizecache + sizeadd);
  }

  j = 0;
  for (i = 0; i < sizecache; i++)
  {
    *(out + j) = *(in + i);
    j++;

    if (*(nl_array + i))
    {
      *(out + j) = '\n';
      j++;
    }
  }

  printf("\n%s\n\n", out);
  free(nl_array);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_loaddict_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_loaddict(int argc, char **argv)
{
  actionu_argchk(1)

  load_dict(*argv);

  if (dict != NULL)  return CFSH_OK;
  else               return CFSH_COMMAND_HARDFAIL;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

