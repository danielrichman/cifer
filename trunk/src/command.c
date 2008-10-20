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

  /* DEBUG */
  printf("command: %s\n", command);
  for (i = 0; i < execinfo->argc; i++)
    printf(" argv[%2i]  %s\n", i, *((execinfo->argv) + i));
  /* End of debug */

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
  /* eg.
  cfsh_func("quit",        cfsh_quit)
  cfsh_func("load",        cfsh_load)
  cfsh_func("affine",      cfsh_affine_dec)
  */

  /* Else... */
  return CFSH_FUNC_NOEXIST;
}

