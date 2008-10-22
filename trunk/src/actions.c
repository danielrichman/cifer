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

#define actionu_argchk(i, u)                                                 \
    if (argc != (i))                                                         \
    {                                                                        \
      printf(u);                                                             \
      return CFSH_COMMAND_SOFTFAIL;                                          \
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

#define actionu_bufferparse(s, t, u)                                         \
    {                                                                        \
      t = actionu_bufferparsef(s);                                           \
      if (t == ACTIONU_BUFFERPARSE_FAIL)                                     \
      {                                                                      \
        printf("invalid buffer name '%s', use 'buffer_#' (X, int 0 to %i)n", \
                               s, cfsh_num_buffers);                         \
        printf(u);                                                           \
        return CFSH_COMMAND_HARDFAIL;                                        \
      }                                                                      \
    }                                                                        \

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
  actionu_intparse_setup();

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
  actionu_intparse_setup();

  actionu_argchk(2,                       action_resize_usage);
  actionu_intparse(*(argv + 1), newsize,  action_resize_usage);
  actionu_bufferparse(*(argv), buffer_id, action_resize_usage);

  if (newsize < 1) 
  {
    printf(action_resize_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  resizebuffer(buffer_id, newsize);
  return CFSH_OK;
}

int action_clear(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1,                       action_clear_usage);
  actionu_bufferparse(*(argv), buffer_id, action_clear_usage);
  clearbuffer(buffer_id);
  return CFSH_OK;
}

int action_copy(int argc, char **argv)
{
  int buffer_id_1, buffer_id_2;
  actionu_argchk(2,                       action_copy_usage);
  actionu_bufferparse(*(argv),     buffer_id_1, action_copy_usage);
  actionu_bufferparse(*(argv + 1), buffer_id_2, action_copy_usage);
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
    return CFSH_COMMAND_SOFTFAIL;
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
      return CFSH_COMMAND_SOFTFAIL;
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
  printf("  Buffer %i - %i bytes. Filter %s. \n\n%s\n", buffer_id, 
          get_buffer_size(buffer_id), get_buffer_filter_text(buffer_id),
          get_buffer(buffer_id));
  return CFSH_OK;
}

int action_bufferinfo(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1,                       action_bufferinfo_usage);
  actionu_bufferparse(*(argv), buffer_id, action_bufferinfo_usage);
  printf("  Buffer %i - %i bytes. Filter %s. \n", buffer_id, 
         get_buffer_size(buffer_id), get_buffer_filter_text(buffer_id));
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
  return CFSH_OK;
}

int action_quickscript(int argc, char **argv)
{
  return CFSH_OK;
}

int action_softscript(int argc, char **argv)
{
  return CFSH_OK;
}

int action_quicksoftscript(int argc, char **argv)
{
  return CFSH_OK;
}

int action_score(int argc, char **argv)
{
  return CFSH_OK;
}

int action_loaddict(int argc, char **argv)
{
  return CFSH_OK;
}

int action_affine(int argc, char **argv)
{
  return CFSH_OK;
}

int action_affinesolve(int argc, char **argv)
{
  return CFSH_OK;
}

int action_affinebf(int argc, char **argv)
{
  return CFSH_OK;
}

int action_bacon(int argc, char **argv)
{
  return CFSH_OK;
}

int action_shift(int argc, char **argv)
{
  return CFSH_OK;
}

int action_deltaic(int argc, char **argv)
{
  return CFSH_OK;
}

int action_monoalph(int argc, char **argv)
{
  return CFSH_OK;
}

int action_ctrans(int argc, char **argv)
{
  return CFSH_OK;
}

int action_fg(int argc, char **argv)
{
  return CFSH_OK;
}

int action_ifg(int argc, char **argv)
{
  return CFSH_OK;
}

int action_fa(int argc, char **argv)
{
  return CFSH_OK;
}

int action_digrams(int argc, char **argv)
{
  return CFSH_OK;
}

int action_trigrams(int argc, char **argv)
{
  return CFSH_OK;
}

int action_pct(int argc, char **argv)
{
  return CFSH_OK;
}

int action_keyb(int argc, char **argv)
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

int action_keyd(int argc, char **argv)
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
  return CFSH_OK;
}

#define cfsh_func(n, f)                                                     \
   if (&f != last_command)                                                  \
   {                                                                        \
     if (aliases_print) printf("\n");                                       \
     printf( f ## _use );                                                   \
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

