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

void cfsh_init()
{
  /* Crucial startup routines. Basically initialises global variables. */
  buffers_init();
  init_dict();
}

void cfsh_autoinit()
{
  /* Automatic "we guessed you wanted this" stuff:
   *  Make 10 Buffers,
   *  Load the Dictionary */ 
  create_buffers(10);
  load_dict();
}

int cfsh_read(FILE *read, int mode)
{
  /* Enter the Loop, accepting stuff from read and passing to cfsh_line */
  int result, looping, linesize, i, num, have_nl;
  char *line;

  /* Start out at 512 chars */
  line = malloc_good( 512 );
  linesize = 512;

  /* Gogogo! */
  looping = 1;
  num = 0;

  /* Shouldn't happen */
  if (mode == CFSH_READ_MODE_INTERACTIVE && read != stdin)
  {
    return CFSH_OK;
  }

  while (looping && feof(read) == 0)
  {
    if (mode == CFSH_READ_MODE_INTERACTIVE) printf("cifer> ");

    *line = 0;  /* Set the first byte so */
    have_nl = 0;

    for (i = 0; feof(read) == 0 && !have_nl; i++)
    {
      if (i >= linesize - 1)
      {
        line = realloc_good( line, linesize + 512 );
        linesize += 512;
      }

      *(line + i) = fgetc(read);

      if (IS_NEWLINE(*(line + i)) || *(line + i) == -1)
      {
        *(line + i) = 0;
        have_nl = 1;
      }
    }

    if (strtlen(line) != 0)
    {
      /* If its not from stdin, then echo it out */
      if (mode != CFSH_READ_MODE_INTERACTIVE && 
          mode != CFSH_READ_MODE_PARSECHECK)
        printf("cifer> %s\n", line);

      /* If a EOF was fired in interactive mode, an extra \n is 
       * needed for niceness */
      if (mode == CFSH_READ_MODE_INTERACTIVE && feof(read) != 0) printf("\n");

      /* Execute/parse and update loop status from that */
      switch (mode)
      {
        case CFSH_READ_MODE_PARSECHECK:
          result = cfsh_line(line, mode);
          if (result == CFSH_COMMAND_PARSEFAIL)
          {
            printf("cfsh_read: parse reports error on line %i.\n", num);
            looping = 0;
          }
          break;
        case CFSH_READ_MODE_EXECUTE_SF:
          result = cfsh_line(line, mode);
          if (result == CFSH_COMMAND_HARDFAIL || 
              result == CFSH_COMMAND_SOFTFAIL ||
              result == CFSH_COMMAND_PARSEFAIL)
          {
            printf("cfsh_read: exit requested by cfsh_line on softhard err.\n");
            looping = 0;
          }
          break;
        case CFSH_READ_MODE_EXECUTE_HF:
          result = cfsh_line(line, mode);
          if (result == CFSH_COMMAND_HARDFAIL)
          {
            printf("cfsh_read: exit requested by cfsh_line on hard err.\n");
            looping = 0;
          }
          break;
        case CFSH_READ_MODE_INTERACTIVE:
          result = cfsh_line(line, mode);
          break;
      }

      if (result == CFSH_BREAK_LOOP)
      {
        printf("cfsh_read: break_loop requested (graceful)\n");
        looping = 0;
      }

      num++;
    }
  }

  if (mode != CFSH_READ_MODE_PARSECHECK || result == CFSH_COMMAND_PARSEFAIL)
    printf("cfsh_read: read %i lines.\n", num);

  if ((mode != CFSH_READ_MODE_PARSECHECK || result == CFSH_COMMAND_PARSEFAIL) &&
      feof(read) != 0)  printf("cfsh_read: end of file\n");
  if (!looping)         printf("cfsh_read: loop broken by cfsh_line\n");

  free(line);

  /* For Parsecheck, the last result will either 
   * be the error or all are ok. Otherwise, this means that
   * the last instruction is the exit code.*/
  return result;
}

void cfsh_scriptfile(char *name, int preparse, int softfail)
{
  /* Execute the file, taking each line and giving it to cfsh_line */
  FILE *read;
  int result;

  read = fopen(name, "r");
  if (read == NULL)
  {
    printf("cfsh_scriptfile: failed to open %s: %s\n", name, strerror(errno));
    return;
  }

  flock(fileno(read), LOCK_SH);

  if (preparse)
  {
    result = cfsh_read(read, CFSH_READ_MODE_PARSECHECK);
    if (result == CFSH_COMMAND_PARSEFAIL)
      printf("cfsh_scriptfile: parse error in file, not executing.\n");

    rewind(read);
  }
  else
  {
    result = CFSH_OK;
  }

  if (result == CFSH_OK)
  {
    if (softfail) result = cfsh_read(read, CFSH_READ_MODE_EXECUTE_SF);
    else          result = cfsh_read(read, CFSH_READ_MODE_EXECUTE_HF);
  }

  flock(fileno(read), LOCK_UN);
  fclose(read);
}

int cfsh_line(char *input, int mode)
{
  /* Use command.c to parse it, execute it and free it. command.c functions
   * won't produce output or errors to stdout, so you must do that here. */ 
  cfsh_execinfo execinfo;
  struct timeval time1, time2;
  int result;

  result = cfsh_parse(input, &execinfo);

  /* No need to break, we has return 
   * Also, there's no need for free_execinfo because if we
   * parsefail, nothing should have been allocated. */
  switch (result)
  {
    case CFSH_PARSE_EBAD:
      if (mode == CFSH_READ_MODE_PARSECHECK) printf("\n%s\n", input);
      printf("cfsh_parse: escape sequence misuse\n");
      if (mode != CFSH_READ_MODE_PARSECHECK) printf("\n");
      return CFSH_COMMAND_PARSEFAIL;
    case CFSH_PARSE_EMPTY:
      if (mode == CFSH_READ_MODE_PARSECHECK) printf("\n%s\n", input);
      printf("cfsh_parse: no command specified\n");
      if (mode != CFSH_READ_MODE_PARSECHECK) printf("\n");
      return CFSH_COMMAND_PARSEFAIL;
    case CFSH_PARSE_QUOTEOPEN:
      if (mode == CFSH_READ_MODE_PARSECHECK) printf("\n%s\n", input);
      printf("cfsh_parse: quotes left open/unclosed.\n");
      if (mode != CFSH_READ_MODE_PARSECHECK) printf("\n");
      return CFSH_COMMAND_PARSEFAIL;
    case CFSH_FUNC_NOEXIST:
      if (mode == CFSH_READ_MODE_PARSECHECK) printf("\n%s\n", input);
      printf("cfsh_parse: no such command or function\n");
      if (mode != CFSH_READ_MODE_PARSECHECK) printf("\n");
      return CFSH_COMMAND_PARSEFAIL;
  }

  if (mode == CFSH_READ_MODE_PARSECHECK)  return CFSH_OK;

  /* gettimeofday should never fail */
  gettimeofday(&time1, NULL);
  result = cfsh_exec(execinfo);
  gettimeofday(&time2, NULL);

  /* Free up */
  cfsh_free_execinfo(&execinfo);

  /* timing info. note: microsecond = one millionth */
  /* Subtract time1 from time2 */
  time2.tv_sec  -= time1.tv_sec;
  time2.tv_usec -= time1.tv_usec;

  /* Only print if its significant */
  if (time2.tv_sec > 0)
  {
    printf("cfsh_line: command spent %6li.%.6li seconds.\n", 
                              time2.tv_sec, time2.tv_usec);
    /* Use printf to provide 000s on the left of usec, to make it correct ;) */
  }
  /* nice spacing =) */
  printf("\n");

  return result;
}
