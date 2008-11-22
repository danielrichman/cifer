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

/*     CFSH_OK                     0
 *     CFSH_BREAK_LOOP             99 */

/* Incoming preprocessor abuse. */

#define actionu_argchk(i, u)                                                 \
    if (argc != (i))                                                         \
    {                                                                        \
      printf(u);                                                             \
      return CFSH_COMMAND_HARDFAIL;                                          \
    }

#define actionu_argless(u)     actionu_argchk(0, u)
#define actionu_intparse_setup()                                             \
      size_t intparse_sz;                                                    \
      char *intparse_validator;                                              \

#define actionu_intparse(s, t, u)                                            \
    {                                                                        \
      intparse_sz = strlen(s);                                               \
      t = strtol((s) + strlefts((s), intparse_sz), &intparse_validator, 10); \
      if (intparse_validator !=                                              \
                       ((s) + intparse_sz - strrights((s), intparse_sz)))    \
      {                                                                      \
         printf(u);                                                          \
         return CFSH_COMMAND_HARDFAIL;                                       \
      }                                                                      \
    }                                                                        \

#define actionu_intparse_char(s, t, u)                                       \
    {                                                                        \
      intparse_sz = strlen(s);                                               \
      if (strtlens(s, intparse_sz) == 1 &&                                   \
           ALPHA_CH( *(s + strlefts(s, intparse_sz)) ))                      \
        t = CHARNUM( *(s + strlefts(s, intparse_sz)) );                      \
      else                                                                   \
        actionu_intparse(s, t, u);                                           \
    }

#define actionu_intparsef(s, t, u, f)                                        \
    {                                                                        \
      intparse_sz = strlen(s);                                               \
      t = strtol((s) + strlefts((s), intparse_sz), &intparse_validator, 10); \
      if (intparse_validator !=                                              \
                       ((s) + intparse_sz - strrights((s), intparse_sz)))    \
      {                                                                      \
         printf(u);                                                          \
         free(f);                                                            \
         return CFSH_COMMAND_HARDFAIL;                                       \
      }                                                                      \
    }                                                                        \

#define actionu_intparsef_char(s, t, u, f)                                   \
    {                                                                        \
      intparse_sz = strlen(s);                                               \
      if (strtlens(s, intparse_sz) == 1 &&                                   \
           ALPHA_CH( *(s + strlefts(s, intparse_sz)) ))                      \
        t = CHARNUM( *(s + strlefts(s, intparse_sz)) );                      \
      else                                                                   \
        actionu_intparsef(s, t, u, f);                                       \
    }

#define actionu_bufferparse(s, t, u)                                         \
    {                                                                        \
      t = actionu_bufferparsef(s);                                           \
      if (t == ACTIONU_BUFFERPARSE_FAIL)                                     \
      {                                                                      \
        printf("bad buffer name '%s', use 'buffer_#' (X, int 0 to %i)\n",    \
                               s, cfsh_num_buffers);                         \
        printf(u);                                                           \
        return CFSH_COMMAND_HARDFAIL;                                        \
      }                                                                      \
    }                                                                        \

#define actionu_bufferchk(i, o, u)                                           \
    if (get_buffer_real_size(i) == 0)                                        \
    {                                                                        \
      printf("bad input buffer: no data.\n");                                \
      printf(u);                                                             \
      return CFSH_COMMAND_HARDFAIL;                                          \
    }                                                                        \
                                                                             \
    if (i == o)                                                              \
    {                                                                        \
      printf("bad output buffer: same as input buffer.\n");                  \
      printf(u);                                                             \
      return CFSH_COMMAND_HARDFAIL;                                          \
    }

#define actionu_bufferfchk(b, f, u)                                          \
    if (get_buffer_filter(b) != f)                                           \
    {                                                                        \
      printf("bad input buffer: filter type '%s' should be '%s'. \n",        \
                    get_buffer_filter_text(b), get_filter_text(f) );         \
      printf(u);                                                             \
      return CFSH_COMMAND_HARDFAIL;                                          \
    }

#define actionu_bufferschk(i, o, u)                                          \
    if (get_buffer_real_size(i) > get_buffer_size(o))                        \
    {                                                                        \
      printf("auto expanding output buffer %i to %i bytes.\n", o,            \
                    get_buffer_real_size(i));                                \
      resizebuffer(o, get_buffer_real_size(i));                              \
    }

#define actionu_copysize(i, o)                                               \
    {                                                                        \
      *(get_buffer(o) + get_buffer_real_size(i)) = 0;                        \
      setbuffernull(o);                                                      \
    }

#define actionu_dictcheck()                                                  \
    if (dict == NULL)                                                        \
    {                                                                        \
      printf("dictionary not loaded. use `loaddict`\n");                     \
      return CFSH_COMMAND_HARDFAIL;                                          \
    }


int actionu_bufferparsef(char *str)
{
  /* strlen("buffer_") = 7 */
  size_t sz;
  long j;
  char *validator;

  sz = strlen(str);
  if (strtlens(str, sz) < 8)                   return ACTIONU_BUFFERPARSE_FAIL;
  if (strncasecmp(str, "buffer_", 7) != 0)     return ACTIONU_BUFFERPARSE_FAIL;

  j = strtol(str + strlefts(str, sz) + 7, &validator, 10);

  if (validator != (str + sz - strrights(str, sz))) 
                                               return ACTIONU_BUFFERPARSE_FAIL;
  if (j > cfsh_num_buffers)                    return ACTIONU_BUFFERPARSE_FAIL;
  if (j < 0)                                   return ACTIONU_BUFFERPARSE_FAIL;

  return j;
}

int action_buffers(int argc, char **argv)
{
  int num;
  actionu_intparse_setup() 

  actionu_argchk(1,              action_buffers_usage);
  actionu_intparse(*(argv), num, action_buffers_usage);

  if (num < 0)
  {
    printf(action_buffers_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  create_buffers(num);
  return CFSH_OK;
}

int action_quit(int argc, char **argv)
{
  actionu_argless(action_quit_usage);
  return CFSH_BREAK_LOOP;
}

int action_resize(int argc, char **argv)
{
  int newsize, buffer_id;
  actionu_intparse_setup() 

  actionu_argchk(2,                       action_resize_usage);
  actionu_intparse(*(argv + 1), newsize,  action_resize_usage);
  actionu_bufferparse(*(argv), buffer_id, action_resize_usage);

  if (newsize < 1) 
  {
    printf(action_resize_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  printf("resizebuffer: resizing buffer_%i to %i bytes...\n", 
                                      buffer_id, newsize);
  resizebuffer(buffer_id, newsize);
  return CFSH_OK;
}

int action_clear(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1,                       action_clear_usage);
  actionu_bufferparse(*(argv), buffer_id, action_clear_usage);
  printf("clearbuffer: clearing buffer_%i...\n", buffer_id);
  clearbuffer(buffer_id);
  return CFSH_OK;
}

int action_copy(int argc, char **argv)
{
  int buffer_id_1, buffer_id_2;
  actionu_argchk(2,                             action_copy_usage);
  actionu_bufferparse(*(argv),     buffer_id_1, action_copy_usage);
  actionu_bufferparse(*(argv + 1), buffer_id_2, action_copy_usage);
  actionu_bufferchk(buffer_id_1, buffer_id_2,   action_copy_usage);
  copybuffer(buffer_id_1, buffer_id_2);
  return CFSH_OK;
}

int action_load(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(2,                           action_load_usage);
  actionu_bufferparse(*(argv + 1), buffer_id, action_load_usage);
  file2buffer(*(argv), buffer_id);
  return CFSH_OK;
}

int action_write(int argc, char **argv)
{
  int mode, buffer_id;

  if (argc != 2 && argc != 3)
  {
    printf(action_write_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

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

  actionu_bufferparse(*(argv + 1), buffer_id, action_write_usage);
  buffer2file(*(argv), buffer_id, mode);

  return CFSH_OK;
}

int action_filter(int argc, char **argv)
{
  int mode, buffer_id;
  actionu_argchk(2,                       action_filter_usage);
  actionu_bufferparse(*(argv), buffer_id, action_filter_usage);
  mode = get_buffer_filter_fromtext(*(argv + 1));
  filterbuffer(buffer_id, mode);
  return CFSH_OK;
}

int action_read(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1,                       action_read_usage);
  actionu_bufferparse(*(argv), buffer_id, action_read_usage);
  printf("  Buffer %i: %i/%i bytes. Filter %s. \n\n", buffer_id, 
         get_buffer_real_size(buffer_id), get_buffer_size(buffer_id),
         get_buffer_filter_text(buffer_id));
  printf("%s\n", get_buffer(buffer_id));
  return CFSH_OK;
}

int action_bufferinfo(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1,                       action_bufferinfo_usage);
  actionu_bufferparse(*(argv), buffer_id, action_bufferinfo_usage);
  printf("  Buffer %i: %i/%i bytes. Filter %s. \n", buffer_id, 
         get_buffer_real_size(buffer_id), get_buffer_size(buffer_id),
         get_buffer_filter_text(buffer_id));
  return CFSH_OK;
}

int action_nullbuffer(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1,                       action_nullbuffer_usage);
  actionu_bufferparse(*(argv), buffer_id, action_nullbuffer_usage);
  setbuffernull(buffer_id);
  printf("set buffer %i's safety-null byte.\n", buffer_id);
  return CFSH_OK;
}

int action_script(int argc, char **argv)
{
  actionu_argchk(1, action_script_usage);
  cfsh_scriptfile(*argv, 0, 0);
  return CFSH_OK;
}

int action_quickscript(int argc, char **argv)
{
  actionu_argchk(1, action_script_usage);
  cfsh_scriptfile(*argv, 1, 0);
  return CFSH_OK;
}

int action_softscript(int argc, char **argv)
{
  actionu_argchk(1, action_script_usage);
  cfsh_scriptfile(*argv, 0, 1);
  return CFSH_OK;
}

int action_quicksoftscript(int argc, char **argv)
{
  actionu_argchk(1, action_script_usage);
  cfsh_scriptfile(*argv, 1, 1);
  return CFSH_OK;
}

int action_score(int argc, char **argv)
{
  int buffer_id, sizecache;
  actionu_argchk(1,                       action_score_usage);
  actionu_bufferparse(*(argv), buffer_id, action_score_usage);
  sizecache = get_buffer_real_size(buffer_id);
  actionu_dictcheck();

  printf("  Buffer %i: %i/%i bytes. Filter %s. \n", buffer_id,
         sizecache, get_buffer_size(buffer_id),
         get_buffer_filter_text(buffer_id));
  printf("  Dictionary Score: %i/%i \n", 
         score_text_dict_fast(get_buffer(buffer_id), sizecache), sizecache);

  return CFSH_OK;
}

int action_spaces(int argc, char **argv)
{
  int buffer_in, buffer_out, sizecache, sizeadd, i, j;
  int *space_array;
  char *in, *out;

  actionu_argchk(2,                                action_spaces_usage);
  actionu_bufferparse(*(argv)    , buffer_in,      action_spaces_usage);
  actionu_bufferparse(*(argv + 1), buffer_out,     action_spaces_usage);
  actionu_bufferchk(buffer_in, buffer_out,         action_spaces_usage);
  actionu_bufferfchk(buffer_in, BUFFER_FILTER_ESP, action_spaces_usage);
  actionu_dictcheck();

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

int action_wordwrap(int argc, char **argv)
{
  int buffer_in, buffer_out, sizecache, sizeadd, i, j;
  int *nl_array;
  char *in, *out;

  actionu_argchk(2,                                action_wordwrap_usage);
  actionu_bufferparse(*(argv)    , buffer_in,      action_wordwrap_usage);
  actionu_bufferparse(*(argv + 1), buffer_out,     action_wordwrap_usage);
  actionu_bufferfchk(buffer_in, BUFFER_FILTER_ENL, action_wordwrap_usage);
  actionu_bufferchk(buffer_in, buffer_out,         action_wordwrap_usage);

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

int action_loaddict(int argc, char **argv)
{
  actionu_argless(action_loaddict_usage);

  load_dict();
  if (dict != NULL)  return CFSH_OK;
  else               return CFSH_COMMAND_HARDFAIL;
}

int action_dictlocation(int argc, char **argv)
{
  actionu_argchk(1, action_dictlocation_usage);

  dict_location = realloc_good(dict_location, strlen(*(argv)) + 1);
  memcpy(dict_location, *(argv), strlen(*(argv)));
  *(dict_location + strlen(*(argv))) = 0;

  printf("dictlocation: set dict_location to be '%s'\n", dict_location);
  return CFSH_OK;
}

int action_affine(int argc, char **argv)
{
  int buffer_in, buffer_out;
  actionu_argchk(2,                            action_affine_usage);
  actionu_bufferparse(*(argv),     buffer_in,  action_affine_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_affine_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_affine_usage);
  actionu_bufferschk(buffer_in, buffer_out,    action_affine_usage);
  actionu_copysize(buffer_in, buffer_out);
  crack_affine(get_buffer(buffer_in), get_buffer_real_size(buffer_in), 
               get_buffer(buffer_out));
  actionu_copysize(buffer_in, buffer_out);
  return CFSH_OK;
}

int action_affinesolve(int argc, char **argv)
{
  int ct1, pt1, ct2, pt2, a, b;
  actionu_intparse_setup() 
  actionu_argchk(4,                         action_affinesolve_usage);
  actionu_intparse_char( *(argv)     , ct1, action_affinesolve_usage);
  actionu_intparse_char( *(argv + 1) , pt1, action_affinesolve_usage);
  actionu_intparse_char( *(argv + 2) , ct2, action_affinesolve_usage);
  actionu_intparse_char( *(argv + 3) , pt2, action_affinesolve_usage);
  affine_solve(ct1, pt1, ct2, pt2, &a, &b);
  return CFSH_OK;
}

int action_affinebf(int argc, char **argv)
{
  int buffer_in, buffer_out;
  actionu_argchk(2,                            action_affinebf_usage);
  actionu_bufferparse(*(argv),     buffer_in,  action_affinebf_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_affinebf_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_affinebf_usage);
  actionu_bufferschk(buffer_in, buffer_out,    action_affinebf_usage);
  actionu_copysize(buffer_in, buffer_out);
  affine_bf(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
            get_buffer(buffer_out));
  actionu_copysize(buffer_in, buffer_out);
  return CFSH_OK;
}

int action_affineencode(int argc, char **argv)
{
  int buffer_in, buffer_out, a, b;
  actionu_intparse_setup() 
  actionu_argchk(4,                            action_affineencode_usage);

  actionu_bufferparse(*(argv),     buffer_in,  action_affineencode_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_affineencode_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_affineencode_usage);
  actionu_bufferschk(buffer_in, buffer_out,    action_affineencode_usage);

  actionu_intparse(   *(argv + 2), a,          action_affineencode_usage);
  actionu_intparse(   *(argv + 3), b,          action_affineencode_usage);

  affine_encode(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                get_buffer(buffer_out), a, b);
  actionu_copysize(buffer_in, buffer_out);

  printf("affineencode: encrypting using the affine cipher, %ix + %i\n\n", 
                    a, b);
  printf("%s\n\n", get_buffer(buffer_out));

  return CFSH_OK;
}

int action_affinedecode(int argc, char **argv)
{
  int buffer_in, buffer_out, a, b;
  actionu_intparse_setup() 
  actionu_argchk(4,                            action_affinedecode_usage);

  actionu_bufferparse(*(argv),     buffer_in,  action_affinedecode_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_affinedecode_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_affinedecode_usage);
  actionu_bufferschk(buffer_in, buffer_out,    action_affinedecode_usage);

  actionu_intparse(   *(argv + 2), a,          action_affinedecode_usage);
  actionu_intparse(   *(argv + 3), b,          action_affinedecode_usage);

  affine_encode(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                get_buffer(buffer_out), a, b);
  actionu_copysize(buffer_in, buffer_out);

  printf("affinedecode: decrypting using the affine cipher, %ix + %i\n\n",
                    a, b);
  printf("%s\n\n", get_buffer(buffer_out));

  return CFSH_OK;
}

int action_baconencode(int argc, char **argv)
{
  int buffer_in, buffer_out;
  actionu_argchk(2,                            action_baconencode_usage);
  actionu_bufferparse(*(argv),     buffer_in,  action_baconencode_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_baconencode_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_baconencode_usage);

  if (get_buffer_real_size(buffer_in) * 5 > get_buffer_size(buffer_out))
  {
    printf("auto expanding output buffer %i to %i bytes (just in case).\n",
                 buffer_out, get_buffer_real_size(buffer_in) * 5);
    resizebuffer(buffer_out, get_buffer_real_size(buffer_in) * 5);
  }

  bacon_encode(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
               get_buffer(buffer_out));
  return CFSH_OK;
}

int action_bacondecode(int argc, char **argv)
{
  int buffer_in, buffer_out;
  actionu_argchk(2,                            action_bacondecode_usage);
  actionu_bufferparse(*(argv),     buffer_in,  action_bacondecode_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_bacondecode_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_bacondecode_usage);

  if (get_buffer_real_size(buffer_in) > get_buffer_size(buffer_out))
  {  
    printf("auto expanding output buffer %i to %i bytes (just in case).\n",
                  buffer_out, get_buffer_real_size(buffer_in));  
    resizebuffer(buffer_out, get_buffer_real_size(buffer_in));  
  }

  bacon_decode(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
               get_buffer(buffer_out));
  return CFSH_OK;
}

int action_shift(int argc, char **argv)
{
  int i, j, k, l, buffer_in, buffer_out, direction;
  char ch;
  int *shift;
  char *dirstr;
  actionu_intparse_setup() 

  if (argc < 4)
  {
    printf(action_shift_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  actionu_bufferparse(*(argv),     buffer_in,  action_shift_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_shift_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_shift_usage);
  actionu_bufferschk(buffer_in, buffer_out,    action_shift_usage);

  if (strcasecmp(*(argv + 2), "forwards") == 0 ||
      strcasecmp(*(argv + 2), "f") == 0)
  {
    direction = 1;
    dirstr = "forwards";
  }
  else if (strcasecmp(*(argv + 2), "backwards") == 0 ||
           strcasecmp(*(argv + 2), "b") == 0)
  {
    direction = 0;
    dirstr = "backwards";
  }
  else
  {
    printf(action_shift_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  if (ALPHA_CH(*(*(argv + 3))))
  {
    for (i = 3, j = 0; i < argc; i++)
    {
      l = strlen(*(argv + i));
      for (k = 0; k < l; k++)
      {
        ch = *(*(argv + i) + k);
        if (ALPHA_CH(ch))        j++;
        else if (!SPACE_CH(ch))
        {
          printf(action_shift_usage);
          return CFSH_COMMAND_HARDFAIL;
        }
      }
    }

    shift = malloc_good( sizeof(int) * j );

    for (i = 3, j = 0; i < argc; i++)
    {
      l = strlen(*(argv + i));
      for (k = 0; k < l; k++)
      {
        ch = *(*(argv + i) + k);

        if (ALPHA_CH(ch))
        {
          *(shift + j) = CHARNUM(ch);
          j++;
        }
      }
    }
  }
  else
  {
    shift = malloc_good( sizeof(int) * (argc - 3) );

    for (i = 3, j = 0; i < argc; i++, j++)
    {
      actionu_intparsef(*(argv + i), *(shift + j), 
                              action_shift_usage, shift);
      *(shift + j) = modn( *(shift + j) , 26);
    }
  }

  if (direction)   caesar_cipher_enc(get_buffer(buffer_in), 
                                     get_buffer_real_size(buffer_in), 
                                     get_buffer(buffer_out), 
                                     shift, j);
  else             caesar_cipher_dec(get_buffer(buffer_in), 
                                     get_buffer_real_size(buffer_in), 
                                     get_buffer(buffer_out), 
                                     shift, j);
  actionu_copysize(buffer_in, buffer_out);

  printf("shift: shifting %s by %c", dirstr, NUMCHAR(*(shift)));
  for (i = 1; i < j; i++) printf("%c", NUMCHAR(*(shift + i)));
  printf("\n\n");

  printf("%s\n\n", get_buffer(buffer_out));
  return CFSH_OK;
}

int action_deltaic(int argc, char **argv)
{
  int buffer_id;
  double result;

  actionu_argchk(1,                       action_deltaic_usage);
  actionu_bufferparse(*(argv), buffer_id, action_deltaic_usage);

  result = delta_ic(get_buffer(buffer_id), get_buffer_real_size(buffer_id), 1);
  printf("The DELTA_IC of buffer_%i is %f\n", buffer_id, result);
  printf("A typical value for English is %f. buffer_%i differs by %f.\n",
               OPTIMAL_DELTA_IC, buffer_id, diff(result, OPTIMAL_DELTA_IC));

  return CFSH_OK;
}

int action_monoalph(int argc, char **argv)
{
  int i, j, l, ch, buffer_in, buffer_out, szc, direction;
  int translation[26];
  char *dirstr;

  actionu_argchk(4,                            action_monoalph_usage);
  actionu_bufferparse(*(argv),     buffer_in,  action_monoalph_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_monoalph_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_monoalph_usage);
  actionu_bufferschk(buffer_in, buffer_out,    action_monoalph_usage);

  if (strcasecmp(*(argv + 2), "encrypt") == 0 ||
      strcasecmp(*(argv + 2), "e") == 0)
  {
    direction = 1;
    dirstr = "encrypting";
  }
  else if (strcasecmp(*(argv + 2), "decrypt") == 0 ||
           strcasecmp(*(argv + 2), "d") == 0)
  {
    direction = 0;
    dirstr = "decrypting";
  }
  else
  {
    printf(action_monoalph_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  szc = strlen(*(argv + 3));
  if (strtlens(*(argv + 3), szc) != 26)
  {
    printf("monoalphabetic translation alphabet should be 26 chars long\n");
    printf(action_monoalph_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  for (i = 0; i < 26; i++) translation[i] = -1;

  j = (szc - strrights(*(argv + 3), szc));
  for (i = strlefts(*(argv + 3), szc), l = 0; i < j; i++, l++)
  {
    ch = CHARNUM(*(*(argv + 3) + i));
    if (ch == -1)  break;

    if (direction) translation[l]  = ch;
    else           translation[ch] = l;
  }

  for (i = 0; i < 26; i++) if (translation[i] == -1)
  {
    printf("invalid character in monoalphabetic translation alphabet");
    printf(action_monoalph_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  printf("monoalph: %s with the translation alphabet ", dirstr);
  for (i = 0; i < 26; i++) printf("%c", NUMCHAR(translation[i]));
  printf("\n\n");

  monoalph_substitute(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                      get_buffer(buffer_out), translation);
  actionu_copysize(buffer_in, buffer_out);

  printf("%s\n\n", get_buffer(buffer_out));
  return CFSH_OK;
}

#define actionu_ctrans_setup()                                                \
    size_t ctrans_sz;                                                         \
    char *ctrans_char;                                                        \
    int ctrans_i, ctrans_key_size;                                            \
    int *ctrans_key;                                                          

#define actionu_ctrans_keyword_parse(argc, argv, u)                           \
    {                                                                         \
      /* Start by trying to find out if it is a number key or an alpha one */ \
      ctrans_sz = strlen(*(argv));                                            \
      ctrans_char = (*(argv) + strlefts(*(argv), ctrans_sz));                 \
      if (NUMBER_CH(*ctrans_char))                                            \
      {                                                                       \
        /* Then we can try and parse the whole thing into a key */            \
        ctrans_key_size = argc;                                               \
        ctrans_key = malloc_good( sizeof(int) * ctrans_key_size);             \
                                                                              \
        for (ctrans_i = 0; ctrans_i < ctrans_key_size; ctrans_i++)            \
        {                                                                     \
          actionu_intparsef(*(argv + ctrans_i), *(ctrans_key + ctrans_i),     \
                                                             u, ctrans_key);  \
        }                                                                     \
      }                                                                       \
      else                                                                    \
      {                                                                       \
        /* Check that its the only argument; it should be */                  \
        if (argc != 1)                                                        \
        {                                                                     \
          printf(u);                                                          \
          return CFSH_COMMAND_HARDFAIL;                                       \
        }                                                                     \
                                                                              \
        /* Easy stuff... */                                                   \
        columnar_transposition_text2key(ctrans_char,                          \
                                        strtlens(*(argv), ctrans_sz),         \
                                        &ctrans_key, &ctrans_key_size);       \
                                                                              \
        if (ctrans_key == NULL || ctrans_key_size == 0)                       \
        {                                                                     \
          printf(u);                                                          \
          return CFSH_COMMAND_HARDFAIL;                                       \
        }                                                                     \
      }                                                                       \
                                                                              \
      if (columnar_transposition_verify_key(ctrans_key, ctrans_key_size)      \
                                                                    == -1)    \
      {                                                                       \
        printf("bad key (repeated numbers or out of range)\n");               \
        printf(u);                                                            \
        return CFSH_COMMAND_HARDFAIL;                                         \
      }                                                                       \
    }

#define actionu_ctrans_default(argc, argv, u)                                 \
      int buffer_in, buffer_out;                                              \
      actionu_intparse_setup()                                               \
      actionu_ctrans_setup()                                                 \
                                                                              \
      if (argc < 3)                                                           \
      {                                                                       \
        printf(u);                                                            \
        return CFSH_COMMAND_HARDFAIL;                                         \
      }                                                                       \
                                                                              \
      actionu_bufferparse(*(argv),     buffer_in,  u);                        \
      actionu_bufferparse(*(argv + 1), buffer_out, u);                        \
      actionu_bufferchk(buffer_in, buffer_out,     u);                        \
      actionu_bufferschk(buffer_in, buffer_out,    u);                        \
                                                                              \
      actionu_ctrans_keyword_parse(argc - 2, argv + 2, u);                    \
                                                                              \
      if (get_buffer_filter(buffer_in) == BUFFER_FILTER_NONE ||               \
          get_buffer_filter(buffer_in) == BUFFER_FILTER_ESP  ||               \
          get_buffer_filter(buffer_in) == BUFFER_FILTER_ENL)                  \
      {                                                                       \
        printf("warning: columnar transposition works on raw characters, \n"  \
 "and so any unfiltered newlines or whitespace will be translated as well \n" \
 "You may want to use the filter command \n");                                \
      }

#define actionu_ctrans_invoke(type, dir)                                      \
      columnar_transposition_ ## type (get_buffer(buffer_in),                 \
                                       get_buffer_real_size(buffer_in),       \
                                       get_buffer(buffer_out),                \
                                       ctrans_key,                            \
                                       ctrans_key_size);                      \
      actionu_copysize(buffer_in, buffer_out);                                \
                                                                              \
      printf("columnar transposition: " #dir " using " #type ": \n");         \
      columnar_transposition_keyinfo(ctrans_key, ctrans_key_size);            \
      printf("%s\n\n", get_buffer(buffer_out));

#define actionu_ctrans_bruteforce(argc, argv, type, u)                        \
      int buffer_in, buffer_out, minb, maxb;                                  \
      actionu_intparse_setup()                                                \
                                                                              \
      actionu_argchk(4,                            u);                        \
      actionu_bufferparse(*(argv),     buffer_in,  u);                        \
      actionu_bufferparse(*(argv + 1), buffer_out, u);                        \
      actionu_bufferchk(buffer_in, buffer_out,     u);                        \
      actionu_bufferschk(buffer_in, buffer_out,    u);                        \
                                                                              \
      actionu_intparse(*(argv + 2), minb,          u);                        \
      actionu_intparse(*(argv + 3), maxb,          u);                        \
                                                                              \
      if (maxb - minb < 0)                                                    \
      {                                                                       \
        printf(u);                                                            \
        return CFSH_COMMAND_HARDFAIL;                                         \
      }                                                                       \
                                                                              \
      columnar_transposition_bruteforce(get_buffer(buffer_in),                \
                                        get_buffer_real_size(buffer_in),      \
                                        get_buffer(buffer_out),               \
                                        minb, maxb,                           \
                                        &(columnar_transposition_ ## type));  \
      return CFSH_OK;

int action_ctrans_keyinfo(int argc, char **argv)
{
  actionu_intparse_setup()
  actionu_ctrans_setup()

  if (argc == 0)
  {
    printf(action_ctrans_keyinfo_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  actionu_ctrans_keyword_parse(argc, argv, action_ctrans_keyinfo_usage);

  columnar_transposition_keyinfo(ctrans_key, ctrans_key_size);

  columnar_transposition_flip_key(ctrans_key, ctrans_key_size);
  printf("flipped ");
  columnar_transposition_keyinfo(ctrans_key, ctrans_key_size);

  return CFSH_OK;
}

int action_c2c_encode(int argc, char **argv)
{
  actionu_ctrans_default(argc, argv, action_c2c_encode_usage);
  actionu_ctrans_invoke(col2col, encode);
  return CFSH_OK;
}

int action_c2c_decode(int argc, char **argv)
{
  actionu_ctrans_default(argc, argv, action_c2c_decode_usage);
  columnar_transposition_flip_key(ctrans_key, ctrans_key_size);
  actionu_ctrans_invoke(col2col, decode);
  return CFSH_OK;
}

int action_c2c_bruteforce(int argc, char **argv)
{
  actionu_ctrans_bruteforce(argc, argv, col2col, action_c2c_bruteforce_usage)
}

int action_r2c_encode(int argc, char **argv)
{
  actionu_ctrans_default(argc, argv, action_r2c_encode_usage);
  actionu_ctrans_invoke(row2col, encode);
  return CFSH_OK;
}

int action_r2c_decode(int argc, char **argv)
{
  actionu_ctrans_default(argc, argv, action_r2c_decode_usage);
  columnar_transposition_flip_key(ctrans_key, ctrans_key_size);
  actionu_ctrans_invoke(row2col, decode);
  return CFSH_OK;
}

int action_r2c_bruteforce(int argc, char **argv)
{
  actionu_ctrans_bruteforce(argc, argv, row2col, action_r2c_bruteforce_usage)
}

int action_c2r_encode(int argc, char **argv)
{
  actionu_ctrans_default(argc, argv, action_c2r_encode_usage);
  actionu_ctrans_invoke(col2row, encode);
  return CFSH_OK;
}

int action_c2r_decode(int argc, char **argv)
{
  actionu_ctrans_default(argc, argv, action_c2r_decode_usage);
  columnar_transposition_flip_key(ctrans_key, ctrans_key_size);
  actionu_ctrans_invoke(col2row, decode);
  return CFSH_OK;
}

int action_c2r_bruteforce(int argc, char **argv)
{
  actionu_ctrans_bruteforce(argc, argv, col2row, action_c2r_bruteforce_usage)
}

int action_fg(int argc, char **argv)
{
  int buffer_in, buffer_out;
  char *outtext;

  if (argc != 1 && argc != 2)
  {
    printf(action_fg_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  actionu_bufferparse(*(argv),       buffer_in,  action_fg_usage);
  outtext = NULL;

  if (argc == 2)
  {
    actionu_bufferparse(*(argv + 1), buffer_out, action_fg_usage);
    actionu_bufferchk(buffer_in, buffer_out,     action_fg_usage);
    actionu_bufferschk(buffer_in, buffer_out,    action_fg_usage);
    outtext = get_buffer(buffer_out);
  }

  frequency_guesses(get_buffer(buffer_in), get_buffer_real_size(buffer_in), 
                    outtext, 1);
  return CFSH_OK;
}

int action_ifg(int argc, char **argv)
{
  int buffer_id, text_size, i, m;
  int ifg[26], width[26];
  actionu_intparse_setup() 

  actionu_argchk(1, action_ifg_usage);

  if (**argv == 'b')
  {
    actionu_bufferparse(*argv, buffer_id, action_ifg_usage);
    text_size = get_buffer_real_size(buffer_id);
  }
  else
  {
    actionu_intparse(*argv, text_size,    action_ifg_usage);
  }

  if (text_size < 0) text_size = 0;

  create_identity_frequency_graph(ifg, text_size);
  printf("Printing identity frequency graph for text_size %i\n", text_size);

  print_setup_width(width, &m);
  print_count_width(ifg, width, &m);
  print_finalise_width(width, &m);

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], i);
  printf("\n");

  printf("F|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], ifg[i]);
  printf("\n\n");

  return CFSH_OK;
}

int action_fa(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1,                            action_fg_usage);
  actionu_bufferparse(*(argv),     buffer_id,  action_fg_usage);
  frequency_guesses(get_buffer(buffer_id), get_buffer_real_size(buffer_id), 
                    NULL, 0);
  return CFSH_OK;
}

int action_digrams(int argc, char **argv)
{
  int buffer_id, num_to_show, i;
  digram *tgt;
  actionu_intparse_setup() 

  actionu_argchk(2,                          action_digrams_usage);
  actionu_bufferparse(*(argv),  buffer_id,   action_digrams_usage);
  actionu_intparse(*(argv + 1), num_to_show, action_digrams_usage);

  if (get_buffer_real_size(buffer_id) < 2)
  {
    printf("Input buffer text too small\n");
    printf(action_digrams_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  if (num_to_show < 1)          num_to_show = 1;
  if (num_to_show > (26 * 26))  num_to_show = (26 * 26);
  tgt = malloc_good( sizeof(digram) * num_to_show );
  count_digrams(get_buffer(buffer_id), get_buffer_real_size(buffer_id),
                tgt, num_to_show);

  printf("Showing top %i digram%c..%c\n", num_to_show,
                (num_to_show == 1 ? '.' : 's'),
                (num_to_show == 1 ? ' ' : '.'));

  for (i = 0; i < num_to_show; i++)
  {
    printf("Rank %i)  \t %c%c\n", i, NUMCHAR(tgt[i].digram_ch1),
                                     NUMCHAR(tgt[i].digram_ch2)  );
  }

  printf("\n");

  free(tgt);
  return CFSH_OK;
}

int action_trigrams(int argc, char **argv)
{
  int buffer_id, num_to_show, i;
  trigram *tgt;
  actionu_intparse_setup() 

  actionu_argchk(2,                          action_trigrams_usage);
  actionu_bufferparse(*(argv),  buffer_id,   action_trigrams_usage);
  actionu_intparse(*(argv + 1), num_to_show, action_trigrams_usage);

  if (get_buffer_real_size(buffer_id) < 3)
  {
    printf("Input buffer text too small\n");
    printf(action_trigrams_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  if (num_to_show < 1)          num_to_show = 1;
  if (num_to_show > (26 * 26))  num_to_show = (26 * 26);
  tgt = malloc_good( sizeof(trigram) * num_to_show );
  count_trigrams(get_buffer(buffer_id), get_buffer_real_size(buffer_id),
                tgt, num_to_show);

  printf("Showing top %i trigram%c..%c\n", num_to_show,
                (num_to_show == 1 ? '.' : 's'),
                (num_to_show == 1 ? ' ' : '.'));

  for (i = 0; i < num_to_show; i++)
  {
    printf("Rank %i)  \t %c%c%c\n", i, NUMCHAR(tgt[i].trigram_ch1),
                                       NUMCHAR(tgt[i].trigram_ch2),
                                       NUMCHAR(tgt[i].trigram_ch3)  );
  }

  printf("\n");

  free(tgt);
  return CFSH_OK;
}

int action_pct(int argc, char **argv)
{
  int i;

  printf("C|");
  for (i = 0; i < 26; i++) printf("%2c|", NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%2i|", i);
  printf("\n\n");

  return CFSH_OK;
}

int action_keyb(int argc, char **argv)
{
  int buffer_in, buffer_out;

  actionu_argchk(2,                            action_keyb_usage);
  actionu_bufferparse(*(argv),     buffer_in,  action_keyb_usage);
  actionu_bufferparse(*(argv + 1), buffer_out, action_keyb_usage);
  actionu_bufferchk(buffer_in, buffer_out,     action_keyb_usage);
  actionu_bufferschk(buffer_in, buffer_out,    action_keyb_usage);
  actionu_dictcheck();

  keyword_bruteforce(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                     get_buffer(buffer_out));

  return CFSH_OK;
}

int action_keye(int argc, char **argv)
{
  return CFSH_OK;
}

int action_keyd(int argc, char **argv)
{
  return CFSH_OK;
}

int action_keyt(int argc, char **argv)
{
  return CFSH_OK;
}

int action_keytf(int argc, char **argv)
{
  return CFSH_OK;
}

int action_polybius(int argc, char **argv)
{
  return CFSH_OK;
}

int action_rfbf(int argc, char **argv)
{
  return CFSH_OK;
}

int action_vigenere(int argc, char **argv)
{
  return CFSH_OK;
}

int action_mmi(int argc, char **argv)
{
  return CFSH_OK;
}

int action_gcd(int argc, char **argv)
{
  return CFSH_OK;
}

int action_coprime(int argc, char **argv)
{
  return CFSH_OK;
}

int action_charinfo(int argc, char **argv)
{
  return CFSH_OK;
}

int action_usage(int argc, char **argv)
{
  actionu_argchk(1, action_usage_usage);
  printf("%s%s", cfsh_get_use(*argv), cfsh_get_usage(*argv));
  return CFSH_OK;
}

#define cfsh_func(n, f)                                                     \
   if (&f != last_command)                                                  \
   {                                                                        \
     if (aliases_print) printf("\n");                                       \
     printf( "\n" );                                                        \
     printf( f ## _use );                                                   \
     printf( f ## _usage );                                                 \
                                                                            \
     last_command = &f;                                                     \
     aliases_print = 0;                                                     \
   }                                                                        \
   else                                                                     \
   {                                                                        \
     if (!aliases_print)                                                    \
     {                                                                      \
       printf("aliases: %s", n);                                            \
       aliases_print = 1;                                                   \
     }                                                                      \
     else                                                                   \
     {                                                                      \
       printf(", %s", n);                                                   \
     }                                                                      \
   }

int action_help(int argc, char **argv)
{
  cfsh_command last_command;
  int aliases_print;

  actionu_argless(action_help_usage);

  aliases_print = 0;
  last_command = NULL;

  #include "command.i"

  if (aliases_print) printf("\n");
  printf("\n");
  printf("type usage <command name> for more information.\n");

  return CFSH_OK;
}

#undef cfsh_func

int action_system(int argc, char **argv)
{
  return CFSH_OK;
}

int action_cd(int argc, char **argv)
{
  return CFSH_OK;
}

int action_ls(int argc, char **argv)
{
  return CFSH_OK;
}
