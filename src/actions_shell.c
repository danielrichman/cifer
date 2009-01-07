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

#define ACTION_USAGE action_quit_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_quit(int argc, char **argv)
{
  actionu_argless()
  return CFSH_BREAK_LOOP;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_usage_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_usage(int argc, char **argv)
{
  actionu_argchk(1)
  printf("%s%s", cfsh_get_use(*argv), cfsh_get_usage(*argv));
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

/* -- A very rudimentary `more' */
/* Checks if we can start a line; blocks if we can't */
void action_help_lnblk(int *lnc, int nln)
{
  if (*lnc == nln)
  {
    printf(" -- Press enter for more -- ");
    while (fgetc(stdin) != '\n');  /* note ; at end for do-nuffing loop */

    *lnc = min(4, nln);  /* Four lines are 'kept' */
  }

  (*lnc)++;
}

/* Echos a potentially multiline-string out. */
void action_help_strpr(char *str, int *lnc, int nln)
{
  int i, j;
  char *str2;
  char *stru;

  j = strlen(str);
  str2 = malloc_good( j + 1 );

  /* Str2 is a copy we can mess with */
  memcpy(str2, str, j);
  str2[j] = 0;

  /* stru is a pointer to the start of the next line, ie. 
   * the last line we printed + 1 */
  stru = str2;

  for (i = 0; i < j; i++)
  {
    if (str[i] == '\n')
    {
      action_help_lnblk(lnc, nln);

      str2[i] = 0;
      printf("%s\n", stru);
      stru = str2 + i + 1;
    }
  }

  if (stru != (str2 + j)) printf("%s", stru);
  free(str2);
}

/* Just a printf("\n"); style thing */
void action_help_nl(int *lnc, int nln)
{
  action_help_lnblk(lnc, nln);
  printf("\n");
}

#define cfsh_func(n, f)                                                     \
   if (&f != last_command)                                                  \
   {                                                                        \
     if (aliases_print)  printf("\n");                                      \
     action_help_nl(&lnc, lines);                                           \
     action_help_strpr( f ## _use,   &lnc, lines );                         \
     action_help_strpr( f ## _usage, &lnc, lines );                         \
                                                                            \
     last_command = &f;                                                     \
     aliases_print = 0;                                                     \
   }                                                                        \
   else                                                                     \
   {                                                                        \
     if (!aliases_print)                                                    \
     {                                                                      \
       action_help_lnblk(&lnc, lines);                                      \
       printf("aliases: %s", n);                                            \
       aliases_print = 1;                                                   \
     }                                                                      \
     else                                                                   \
     {                                                                      \
       printf(", %s", n);                                                   \
     }                                                                      \
   }

#define ACTION_USAGE action_help_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_help(int argc, char **argv)
{
  cfsh_command last_command;
  int aliases_print, lines, lnc;
  struct winsize win;

  /* By default we go for 22, 2 less than the default height (24) */
  lines = 22;

  /* Figure out how many lines there are */
  /* Subtract 2 to leave space for any straggling lines, eg aliases */
  if (ioctl(fileno(stdout), TIOCGWINSZ, &win) != -1)
  {
    if (win.ws_row != 0)    lines = win.ws_row - 2;
  }

  actionu_argless()

  aliases_print = 0;
  lnc = 0;
  last_command = NULL;

  #include "command.inc"

  if (aliases_print) action_help_nl(&lnc, lines);
  action_help_nl(&lnc, lines);
  action_help_strpr("type usage <command name> for more information.\n", 
                                                               &lnc, lines);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL
#undef cfsh_func

#define ACTION_USAGE action_system_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_system(int argc, char **argv)
{
  int ret, retr;
  char *cmd;

  /* TODO: this whole thing is an ugly hack, cfsh_commandline and all */
  /* Use the untainted copy but cut off the first bit */
  cmd = cfsh_commandline;

  /* If argc is -1, its one of the ones below calling me,
   * so don't trim the first bit */
  if (argc != -1)  while (*cmd != ' ' && *cmd != '\0')  cmd++;

  if (cmd == '\0' || strtlen(cmd) == 0) 
  {
    printf("no command specified for system\n");
    return CFSH_COMMAND_HARDFAIL;
  }

  ret = system(cmd);

  /* Todo, fixme! (System spesific) */
  retr = (ret >> 8) & 0xFF;

  if (ret == -1)  printf("system returned -1, failed.\n");
  else            printf("shell exited %i\n", retr);

  return (ret == 0 ? CFSH_OK : CFSH_COMMAND_SOFTFAIL);
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_cd_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_cd(int argc, char **argv)
{
  int result;

  actionu_argchk(1)
  result = chdir(*argv);

  if (result != 0)
  {
    perror("chdir");
    actionu_faili()
  }

  printf("changed directory successfully\n");
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_ls_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_ls(int argc, char **argv)
{
  /* Just send to system ;) */
  action_system(-1, NULL);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

