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

int cfsh_parse(char *input, cfsh_execinfo *execinfo)
{
  /* Parse string line into argc and argv */
  int i, j, size, spacing, had_char, had_command, quote, escaping;
  char *input_mark, *input_end, *command;
  char ch;

  execinfo->argv    = NULL;
  execinfo->command = NULL;

  execinfo->argc   = 0;
  spacing          = 0;
  had_char         = 0;
  had_command      = 0;
  quote            = 0;
  escaping         = 0;

  command          = NULL;
  size             = strlen(input);
  input_end        = input + size;

  /* Pass 1: count the number of arguments and filter bad, nasty characters */
  for (input_mark = input; input_mark < input_end; input_mark++)
  {
    ch = (char) *input_mark;

    if (escaping == 0 && quote == 0 && (SPACE_CH(ch) || XSPACE_CH(ch)))
    {
      if (!spacing && had_char)
      {
        if (had_command)   (execinfo->argc)++;
        else               had_command = 1;

        spacing = 1;   /* Catch multiple spaces */
      }
    }
    else if (escaping == 0 && quote == 0 && QUOTE_CH(ch))
    {
      quote = ch;
    }
    else if (escaping == 0 && ch == quote)
    {
      if (!had_char)
      {
        return CFSH_PARSE_EMPTY;
      }
      else
      {
        if (had_command)   (execinfo->argc)++;
        else               had_command = 1;

        spacing = 1;
      }

      quote = 0;
    }
    else if (escaping == 0 && ESCAPE_CH(ch))
    {
      escaping = 1;
    }
    else if (escaping && !ESCAPE_CH(ch) && !SPACE_CH(ch) && 
                         !XSPACE_CH(ch) && !QUOTE_CH(ch))
    {
      return CFSH_PARSE_EBAD;
    }
    else
    {
      spacing = 0;
      had_char = 1;
      escaping = 0;
    }
  }

  /* And anything left over... */
  if (!spacing && had_char)
  {
    if (had_command)   (execinfo->argc)++;
    else               had_command = 1;
  }

  if (!had_command)    return CFSH_PARSE_EMPTY;
  if (quote != 0)      return CFSH_PARSE_QUOTEOPEN;
  if (escaping)        return CFSH_PARSE_EBAD;

  /* Allocate... */
  if (execinfo->argc != 0)
    execinfo->argv = malloc_good( sizeof(char *) * (execinfo->argc) );

  /* Pass 2: Allocate correct lengths! */
  execinfo->argc   = 0;
  spacing          = 0;
  had_char         = 0;
  had_command      = 0;
  quote            = 0;
  escaping         = 0;
  j                = 0;

  for (input_mark = input; input_mark < input_end; input_mark++)
  {
    ch = (char) *input_mark;

    if (escaping == 0 && quote == 0 && (SPACE_CH(ch) || XSPACE_CH(ch)))
    {
      if (!spacing && had_char)
      {
        if (had_command)
        {
          *((execinfo->argv) + (execinfo->argc)) = malloc_good( j + 1 );
          (execinfo->argc)++;
        }
        else
        {
          command = malloc_good( j + 1 );
          had_command = 1;
        }

        spacing = 1;   /* Catch multiple spaces */
        j = 0;
      }
    }
    else if (escaping == 0 && quote == 0 && QUOTE_CH(ch))
    {
      quote = ch;
    }
    else if (escaping == 0 && ch == quote)
    {
      if (had_command)
      {
        *((execinfo->argv) + (execinfo->argc)) = malloc_good( j + 1 );
        (execinfo->argc)++;
      }
      else
      {
        command = malloc_good( j + 1 );
        had_command = 1;
      }

      spacing = 1;
      quote = 0;
      j = 0;
    }
    else if (escaping == 0 && ESCAPE_CH(ch))
    {
      escaping = 1;
    }
    else
    {
      spacing = 0;
      had_char = 1;
      escaping = 0;
      j++;
    }
  }

  /* And any final left over... */
  if (!spacing && had_char)
  {
    if (had_command)
    {
      *((execinfo->argv) + (execinfo->argc)) = malloc_good( j + 1 );
      (execinfo->argc)++;          j = 0;
    }
    else
    {
      command = malloc_good( j + 1 );
      had_command = 1;             j = 0;
    }

    spacing = 1;   /* Catch multiple spaces */
  }

  /* Pass 3: Load up the values! */
  execinfo->argc   = 0;
  spacing          = 0;
  had_char         = 0;
  had_command      = 0;
  quote            = 0;
  escaping         = 0;
  j                = 0;

  for (input_mark = input; input_mark < input_end; input_mark++)
  {
    ch = (char) *input_mark;

    if (escaping == 0 && quote == 0 && (SPACE_CH(ch) || XSPACE_CH(ch)))
    {
      if (!spacing && had_char)
      {
        if (had_command)
        {
          *( *((execinfo->argv) + execinfo->argc) + j ) = 0;
          (execinfo->argc)++;
        }
        else
        {
          *( command + j ) = 0;
          had_command = 1;
        }

        spacing = 1;   /* Catch multiple spaces */
        j = 0;
      }
    }
    else if (escaping == 0 && quote == 0 && QUOTE_CH(ch))
    {
      quote = ch;
    }
    else if (escaping == 0 && ch == quote)
    {
      if (had_command)   
      {   
        *( *((execinfo->argv) + execinfo->argc) + j ) = 0;
        (execinfo->argc)++;
      }
      else
      {               
        *( command + j ) = 0;
        had_command = 1;
      }

      j = 0;
      spacing = 1;
      quote = 0;
    }
    else if (escaping == 0 && ESCAPE_CH(ch))
    {
      escaping = 1;
    }
    else
    {
      spacing = 0;
      had_char = 1;
      escaping = 0;

      if (had_command)  *( *((execinfo->argv) + execinfo->argc) + j ) = ch;
      else              *( command                              + j ) = ch;

      j++;
    }
  }

  if (!spacing && had_char)
  {
    if (had_command)
    {
      *( *((execinfo->argv) + execinfo->argc) + j ) = 0;
      (execinfo->argc)++;
    }
    else
    {
      *( command + j ) = 0;
      had_command = 1;
    }
  }

  i = cfsh_get_func(command, &(execinfo->command));
  free(command);

  if (i == CFSH_FUNC_OK)
  {
    return CFSH_PARSE_OK;
  }
  else
  {
    cfsh_free_execinfo(execinfo);
    return i;
  }
}

void cfsh_free_execinfo(cfsh_execinfo *execinfo)
{
  int i;

  for (i = 0; i < (execinfo->argc); i++)    free(*((execinfo->argv) + i));
  if (execinfo->argc != 0)                  free(execinfo->argv);
}

#define cfsh_func(n, f)  if (strcasecmp(n, name) == 0) \
                         {  *command = &f;  return CFSH_FUNC_OK; } 

int cfsh_get_func(char *name, cfsh_command *command)
{
  /* cfsh_func("", action_) */
  cfsh_func("buffers"                     , action_buffers)
  cfsh_func("quit"                        , action_quit)
  cfsh_func("resize"                      , action_resize)
  cfsh_func("clear"                       , action_clear)
  cfsh_func("copy"                        , action_copy)
  cfsh_func("load"                        , action_load)
  cfsh_func("write"                       , action_write)
  cfsh_func("filter"                      , action_filter)
  cfsh_func("getsize"                     , action_getsize)
  cfsh_func("getfilter"                   , action_getfilter)
  cfsh_func("nullbuffer"                  , action_nullbuffer)
  cfsh_func("file"                        , action_script)
  cfsh_func("script"                      , action_script)
  cfsh_func("quickfile"                   , action_quickscript)
  cfsh_func("quickscript"                 , action_quickscript)
  cfsh_func("softfile"                    , action_softscript)
  cfsh_func("softscript"                  , action_softscript)
  cfsh_func("quicksoftfile"               , action_quicksoftscript)
  cfsh_func("quicksoftscript"             , action_quicksoftscript)
  cfsh_func("score"                       , action_score)
  cfsh_func("loaddict"                    , action_loaddict)
  cfsh_func("load_dict"                   , action_loaddict)
  cfsh_func("affine"                      , action_affine)
  cfsh_func("crack_affine"                , action_affine)
  cfsh_func("affinesolve"                 , action_affinesolve)
  cfsh_func("affine_solve"                , action_affinesolve)
  cfsh_func("affinebf"                    , action_affinebf)
  cfsh_func("affine_bf"                   , action_affinebf)
  cfsh_func("bacon"                       , action_bacon)
  cfsh_func("shift"                       , action_shift)
  cfsh_func("vigenere_manual"             , action_shift)
  cfsh_func("deltaic"                     , action_deltaic)
  cfsh_func("delta_ic"                    , action_deltaic)
  cfsh_func("monoalph"                    , action_monoalph)
  cfsh_func("ctrans"                      , action_ctrans)
  cfsh_func("fg"                          , action_fg)
  cfsh_func("frequency_guesses"           , action_fg)
  cfsh_func("ifg"                         , action_ifg)
  cfsh_func("identity_frequency_graph"    , action_ifg)
  cfsh_func("fa"                          , action_fa)
  cfsh_func("count_freq"                  , action_fa)
  cfsh_func("frequency"                   , action_fa)
  cfsh_func("frequency_analysis"          , action_fa)
  cfsh_func("digrams"                     , action_digrams)
  cfsh_func("count_digrams"               , action_digrams)
  cfsh_func("trigrams"                    , action_trigrams)
  cfsh_func("count_trigrams"              , action_trigrams)
  cfsh_func("pct"                         , action_pct)
  cfsh_func("chartable"                   , action_pct)
  cfsh_func("keyb"                        , action_keyb)
  cfsh_func("keyword_bruteforce"          , action_keyb)
  cfsh_func("keyt"                        , action_keyt)
  cfsh_func("keyword_table"               , action_keyt)
  cfsh_func("keytf"                       , action_keytf)
  cfsh_func("keyword_table_flipped"       , action_keytf)
  cfsh_func("keyd"                        , action_keyd)
  cfsh_func("keyword_decode"              , action_keyd)
  cfsh_func("polybius"                    , action_polybius)
  cfsh_func("rfbf"                        , action_rfbf)
  cfsh_func("railfence"                   , action_rfbf)
  cfsh_func("rf"                          , action_rfbf)
  cfsh_func("vigenere"                    , action_vigenere)
  cfsh_func("vigenere_crack"              , action_vigenere)
  cfsh_func("mmi"                         , action_mmi)
  cfsh_func("modmulinv"                   , action_mmi)
  cfsh_func("gcd"                         , action_gcd)
  cfsh_func("greatest_common_denominator" , action_gcd)
  cfsh_func("coprime"                     , action_coprime)
  cfsh_func("is_coprime"                  , action_coprime)
  cfsh_func("charinfo"                    , action_charinfo)
  cfsh_func("char_info"                   , action_charinfo)
  cfsh_func("char"                        , action_charinfo)

  /* Else... */
  return CFSH_FUNC_NOEXIST;
}

