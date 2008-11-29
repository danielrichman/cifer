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

#define ACTION_USAGE action_affine_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_affine(int argc, char **argv)
{
  int buffer_in, buffer_out;

  actionu_argchk(2)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

  actionu_copysize(buffer_in, buffer_out);
  crack_affine(get_buffer(buffer_in), get_buffer_real_size(buffer_in), 
               get_buffer(buffer_out));
  actionu_copysize(buffer_in, buffer_out);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_affinesolve_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_affinesolve(int argc, char **argv)
{
  int ct1, pt1, ct2, pt2, a, b;

  actionu_argchk(4)
  actionu_intparse_char( *(argv),     ct1 )
  actionu_intparse_char( *(argv + 1), pt1 )
  actionu_intparse_char( *(argv + 2), ct2 )
  actionu_intparse_char( *(argv + 3), pt2 )

  affine_solve(ct1, pt1, ct2, pt2, &a, &b);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_affinebf_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_affinebf(int argc, char **argv)
{
  int buffer_in, buffer_out;
  actionu_argchk(2)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

  actionu_copysize(buffer_in, buffer_out);
  affine_bf(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
            get_buffer(buffer_out));
  actionu_copysize(buffer_in, buffer_out);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_affineencode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_affineencode(int argc, char **argv)
{
  int buffer_in, buffer_out, a, b;

  actionu_argchk(4)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

  actionu_intparse(*(argv + 2), a)
  actionu_intparse(*(argv + 3), b)

  affine_encode(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                get_buffer(buffer_out), a, b);
  actionu_copysize(buffer_in, buffer_out);

  printf("affineencode: encrypting using the affine cipher, %ix + %i\n\n", 
                    a, b);
  printf("%s\n\n", get_buffer(buffer_out));

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_affinedecode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_affinedecode(int argc, char **argv)
{
  int buffer_in, buffer_out, a, b;

  actionu_argchk(4)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

  actionu_intparse(*(argv + 2), a)
  actionu_intparse(*(argv + 3), b)

  affine_encode(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                get_buffer(buffer_out), a, b);
  actionu_copysize(buffer_in, buffer_out);

  printf("affinedecode: decrypting using the affine cipher, %ix + %i\n\n",
                    a, b);
  printf("%s\n\n", get_buffer(buffer_out));

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_baconencode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_baconencode(int argc, char **argv)
{
  int buffer_in, buffer_out;

  actionu_argchk(2)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)

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
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_bacondecode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_bacondecode(int argc, char **argv)
{
  int buffer_in, buffer_out;
  actionu_argchk(2)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)

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
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_shift_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_shift(int argc, char **argv)
{
  int i, j, k, l, buffer_in, buffer_out, direction;
  char ch;
  int *shift;
  char *dirstr;

  if (argc < 4) actionu_fail()

  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

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
      actionu_intparsef(*(argv + i), *(shift + j), shift)
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
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_monoalph_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_monoalph(int argc, char **argv)
{
  int i, j, l, ch, buffer_in, buffer_out, szc, direction;
  int translation[26];
  char *dirstr;

  actionu_argchk(4)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

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
#undef ACTION_USAGE
#undef ACTION_FAIL

int actionu_ctrans_keyword_parse(int argc, char **argv, 
                                 int **key, int *key_size)
{
  int i;
  size_t sz;
  char *ch;

  /* Start by trying to find out if it is a number key or an alpha one */
  sz = strlen(*(argv));
  ch = (*(argv) + strlefts(*(argv), sz));

  if (NUMBER_CH(*ch))
  {
    /* Then we can try and parse the whole thing into a key */
    *key_size = argc;
    *key = malloc_good(sizeof(int) * argc);

    for (i = 0; i < argc; i++)
    {
      if (actionu_intparse_f(*(argv + i), *key + i, 0, NULL) == -1)
      {
        free(*key);
        *key = NULL;
        *key_size = 0;
        return -1;
      }
    }
  }
  else
  {
    /* Check that its the only argument; it should be */
    if (argc != 1) return -1;

    /* Easy stuff... */
    columnar_transposition_text2key(ch, strtlens(*(argv), sz),
                                    key, key_size);

    if (key == NULL || key_size == 0) return -1;
  }

  if (columnar_transposition_verify_key(*key, *key_size) == -1)
  {
    printf("bad key (repeated numbers or out of range)\n");
    free(*key);
    *key = NULL;
    *key_size = 0;
    return -1;
  }

  return 0;
}

int actionu_ctrans_default(int argc, char **argv, 
                           int flip, char *dirstring, char *typestring,
                           columnar_transposition_function routine)
{
  int buffer_in, buffer_out, filter, key_size;
  int *key;

  if (argc < 3)  return -1;

  buffer_in  = actionu_bufferparse_f(*(argv));
  buffer_out = actionu_bufferparse_f(*(argv + 1));

  if (buffer_in == -1)
  {
    printf(actionu_bufferparse_fail, *(argv), cfsh_num_buffers);
    return -1;
  }

  if (buffer_out == -1)
  {
    printf(actionu_bufferparse_fail, *(argv + 1), cfsh_num_buffers);
    return -1;
  }

  if (!(actionu_bufferchk_f(buffer_in, buffer_out))) return -1;
  actionu_bufferschk(buffer_in, buffer_out);

  if (actionu_ctrans_keyword_parse(argc - 2, argv + 2, &key, &key_size) == -1)
    return -1;

  filter = get_buffer_filter(buffer_in);

  if (filter == BUFFER_FILTER_NONE ||
      filter == BUFFER_FILTER_ESP  ||
      filter == BUFFER_FILTER_ENL)
  {
    printf("warning: columnar transposition works on raw characters, \n"
 "and so any unfiltered newlines or whitespace will be translated as well \n"
 "You may want to use the filter command \n");
  }

  if (flip) columnar_transposition_flip_key(key, key_size);

  routine(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
          get_buffer(buffer_out), key, key_size);
  actionu_copysize(buffer_in, buffer_out);

  printf("columnar transposition: %s using %s: \n", dirstring, typestring);
  columnar_transposition_keyinfo(key, key_size);
  printf("%s\n\n", get_buffer(buffer_out));

  free(key);

  return 0;
}

int actionu_ctrans_bruteforce(int argc, char **argv,
                              columnar_transposition_function routine)
{
  int buffer_in, buffer_out, filter, minb, maxb;

  if (argc != 4)  return -1;

  buffer_in  = actionu_bufferparse_f(*(argv));
  buffer_out = actionu_bufferparse_f(*(argv + 1));

  if (buffer_in == -1)
  {
    printf(actionu_bufferparse_fail, *(argv), cfsh_num_buffers);
    return -1;
  }

  if (buffer_out == -1)
  {
    printf(actionu_bufferparse_fail, *(argv + 1), cfsh_num_buffers);
    return -1;
  }

  if (!(actionu_bufferchk_f(buffer_in, buffer_out)))    return -1;
  actionu_bufferschk(buffer_in, buffer_out);

  if (!actionu_intparse_f(*(argv + 2), &minb, 0, NULL)) return -1;
  if (!actionu_intparse_f(*(argv + 2), &maxb, 0, NULL)) return -1;

  if (maxb - minb < 0) return -1;

  filter = get_buffer_filter(buffer_in);

  if (filter == BUFFER_FILTER_NONE ||
      filter == BUFFER_FILTER_ESP  ||
      filter == BUFFER_FILTER_ENL)
  {
    printf("warning: columnar transposition works on raw characters, \n"
 "and so any unfiltered newlines or whitespace will be translated as well \n"
 "You may want to use the filter command \n");
  }

  columnar_transposition_bruteforce(get_buffer(buffer_in),
                                    get_buffer_real_size(buffer_in),
                                    get_buffer(buffer_out),
                                    minb, maxb, routine);
  actionu_copysize(buffer_in, buffer_out);

  return 0;
}

#define actionu_ctrans_auto(flip, dir, type)                                 \
  actionu_funcchk(actionu_ctrans_default(argc, argv, flip, #dir, #type,      \
                                        &(columnar_transposition_ ## type))) \
  return CFSH_OK;

#define actionu_ctrans_auto_encode(type)  actionu_ctrans_auto(0, encode, type)
#define actionu_ctrans_auto_decode(type)  actionu_ctrans_auto(1, decode, type)

#define actionu_ctrans_bruteforce_auto(type)                                 \
  actionu_funcchk(actionu_ctrans_bruteforce(argc, argv,                      \
                                        &(columnar_transposition_ ## type))) \
  return CFSH_OK;

#define ACTION_USAGE action_ctrans_keyinfo_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_ctrans_keyinfo(int argc, char **argv)
{
  int *key, key_size;

  if (argc == 0) actionu_fail();

  actionu_funcchk(actionu_ctrans_keyword_parse(argc, argv, &key, &key_size))

  columnar_transposition_keyinfo(key, key_size);
  columnar_transposition_flip_key(key, key_size);
  printf("flipped ");
  columnar_transposition_keyinfo(key, key_size);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_c2c_encode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_c2c_encode(int argc, char **argv)
{
  actionu_ctrans_auto_encode(col2col)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_c2c_decode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_c2c_decode(int argc, char **argv)
{
  actionu_ctrans_auto_decode(col2col)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_c2c_bruteforce_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_c2c_bruteforce(int argc, char **argv)
{
  actionu_ctrans_bruteforce_auto(col2col)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_r2c_encode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_r2c_encode(int argc, char **argv)
{
  actionu_ctrans_auto_encode(row2col)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_r2c_decode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_r2c_decode(int argc, char **argv)
{
  actionu_ctrans_auto_decode(row2col)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_r2c_bruteforce_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_r2c_bruteforce(int argc, char **argv)
{
  actionu_ctrans_bruteforce_auto(row2col)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_c2r_encode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_c2r_encode(int argc, char **argv)
{
  actionu_ctrans_auto_encode(col2row)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_c2r_decode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_c2r_decode(int argc, char **argv)
{
  actionu_ctrans_auto_decode(col2row)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_c2r_bruteforce_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_c2r_bruteforce(int argc, char **argv)
{
  actionu_ctrans_bruteforce_auto(col2row)
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_keyb_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_keyb(int argc, char **argv)
{
  int buffer_in, buffer_out;

  actionu_argchk(2)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);
  actionu_dictcheck()

  keyword_bruteforce(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                     get_buffer(buffer_out));
  actionu_copysize(buffer_in, buffer_out);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_keye_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_keye(int argc, char **argv)
{
  int buffer_in, buffer_out, key_size;
  char *keyword;

  actionu_argchk(3)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

  keyword = *(argv + 2);
  keyword += strleft(keyword);
  key_size = strtlen(keyword);

  actionu_funcchk(keyword_check(keyword, key_size));
  keyword_single(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                 get_buffer(buffer_out), keyword, key_size, 0);
  actionu_copysize(buffer_in, buffer_out);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_keyd_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_keyd(int argc, char **argv)
{
  int buffer_in, buffer_out, key_size;
  char *keyword;

  actionu_argchk(3)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

  keyword = *(argv + 2);
  keyword += strleft(keyword);
  key_size = strtlen(keyword);

  actionu_funcchk(keyword_check(keyword, key_size));
  keyword_single(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                 get_buffer(buffer_out), keyword, key_size, 1);
  actionu_copysize(buffer_in, buffer_out);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_keyt_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_keyt(int argc, char **argv)
{
  int key_size;
  int table[26];
  char *keyword;

  actionu_argchk(1)
  keyword = *(argv);
  keyword += strleft(keyword);
  key_size = strtlen(keyword);
  *(keyword + key_size) = 0;

  actionu_funcchk(keyword_check(keyword, key_size));
  keyword_table(keyword, key_size, table);

  printf("Translation table for keyword '%s': \n", keyword);
  keyword_print_keyinfo(table);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_keytf_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_keytf(int argc, char **argv)
{
  int key_size;
  int table[26];
  char *keyword;

  actionu_argchk(1)
  keyword = *(argv);
  keyword += strleft(keyword);
  key_size = strtlen(keyword);
  *(keyword + key_size) = 0;

  actionu_funcchk(keyword_check(keyword, key_size));
  keyword_table_preflipped(keyword, key_size, table);

  printf("Decode (flipped) translation table for keyword '%s': \n", keyword);
  keyword_print_keyinfo(table);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_polybius_encode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_polybius_encode(int argc, char **argv)
{
  int buffer_in, buffer_out;
  actionu_argchk(2)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferfchk(buffer_in, BUFFER_FILTER_ALPHA)

  if (get_buffer_real_size(buffer_in) * 2 > get_buffer_size(buffer_out))
  {
    printf("auto expanding output buffer %i to %i bytes (just in case).\n",
                 buffer_out, get_buffer_real_size(buffer_in) * 2);
    resizebuffer(buffer_out, get_buffer_real_size(buffer_in) * 2);
  }

  polybius_encode(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                  get_buffer(buffer_out));
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_polybius_decode_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_polybius_decode(int argc, char **argv)
{
  int buffer_in, buffer_out;
  actionu_argchk(2)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferfchk(buffer_in, BUFFER_FILTER_NUM)

  if (modp(get_buffer_real_size(buffer_in), 2) == 1)
  {
    printf("input buffer doesn't have an even number of characters.");
    actionu_faili()
  }

  if ((get_buffer_real_size(buffer_in) / 2) > get_buffer_size(buffer_out))
  {
    printf("auto expanding output buffer %i to %i bytes (just in case).\n",
                 buffer_out, (get_buffer_real_size(buffer_in) / 2));
    resizebuffer(buffer_out, (get_buffer_real_size(buffer_in) / 2));
  }

  polybius_decode(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                  get_buffer(buffer_out));
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_rfbf_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_rfbf(int argc, char **argv)
{
  int buffer_in, buffer_out, minb, maxb;

  actionu_argchk(4)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

  actionu_intparse(*(argv + 2), minb)
  actionu_intparse(*(argv + 3), maxb)

  rf_bf(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
        get_buffer(buffer_out), minb, maxb);
  actionu_copysize(buffer_in, buffer_out);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_vigenere_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_vigenere(int argc, char **argv)
{
  int buffer_in, buffer_out, minb, maxb;

  actionu_argchk(4)
  actionu_bufferparse(*(argv),     buffer_in)
  actionu_bufferparse(*(argv + 1), buffer_out)
  actionu_bufferchk(buffer_in, buffer_out)
  actionu_bufferschk(buffer_in, buffer_out);

  actionu_intparse(*(argv + 2), minb)
  actionu_intparse(*(argv + 3), maxb)

  crack_vigenere(get_buffer(buffer_in), get_buffer_real_size(buffer_in),
                 get_buffer(buffer_out), minb, maxb);
  actionu_copysize(buffer_in, buffer_out);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

