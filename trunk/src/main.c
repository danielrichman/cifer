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

int main(int argc, char **argv)
{
  /* Take a look at argc & argv.
   * If argc == 0; go to interatcive mode (shell.c)
   * else, first look for switches:
   *   -n disables auto-init
   *   -i means that the file should be executed THEN go to interactive
   *   -f is a synonym for just using the script instruction on its own
   *   -q means that any file executed shouldn't be wholly parsed first
   *   -s means that soft-fails will cause an exit, not just hard-fails
   * then treat the rest as a shell command.  */

  int i, j, noauto, init, file, quick, soft;
  cfsh_execinfo execinfo;

  noauto = 0;
  init   = 0;
  file   = 0;
  quick  = 0;
  soft   = 0;

  /* Unavoidable startup. Initialises global pointers and sets
   * default dictionary location */
  cfsh_init();

  if (argc < 2)
  {
    cfsh_autoinit();
    return cfsh_interactive();
  }
  else
  {
    /* Are there any sqitches? */
    if (**(argv + 1) == '-')   /* First char of first arg. */
    {
      for (i = 1; i < strlen(*(argv + 1)); i++) switch (*(*(argv + 1) + i))
      {
        case 'n':   noauto = 1;  break;
        case 'i':   init   = 1;  break;
        case 'f':   file   = 1;  break;
        case 'q':   quick  = 1;  break;
        case 's':   soft   = 1;  break;
      }
    }

    if ((init || file) && argc > 3)
    {
      printf("cifer (main): trailing arguments.\n");
      return 1;
    }

    if ((quick || soft) && !(init || file))
    {
      printf("cifer (main): quick || soft specified but no init or file.\n");
      return 1;
    }

    if (init && file)
    {
      printf("cifer (main): init cannot be used in conjection with file.\n");
      return 1;
    }

    /* Depending on if there were switches or not, where do args start ? */
    i = (**(argv + 1) == '-' ? 3 : 2);

    if (init || file)
    {
      if (argc < 3)
      {
        printf("cifer (main): please specify a file.\n");
        return 1;
      }

      if (!noauto) cfsh_autoinit();
      cfsh_scriptfile(*(argv + 2), !quick, soft);
      if (init) cfsh_interactive();
    }
    else if (argc > i - 1)
    {
      if (!noauto) cfsh_autoinit();
      execinfo.argc = argc - i;

      if (execinfo.argc != 0)
        execinfo.argv = malloc_good( sizeof(char *) * execinfo.argc );
      else
        execinfo.argv = NULL;

      if (cfsh_get_func(*(argv + i - 1), &execinfo.command) 
             == CFSH_FUNC_NOEXIST)
      {
        printf("cifer (main): command does not exist.\n");
        return 1;
      }

      for (j = 0; j < execinfo.argc; j++)  
        *(execinfo.argv + j) = *(argv + j + i);

      return cfsh_exec(execinfo);
    }
    else
    {
      if (!noauto) cfsh_autoinit();
      return cfsh_interactive();
    }
  }

  return 0;  /* This should return whatever shell.c ends up with */
} 
