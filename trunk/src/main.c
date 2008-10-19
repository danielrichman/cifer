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
   *   -n --noauto disables auto-init
   *   -f --file means that the file should be executed THEN go to interactive
   *   -d --delay means that any file executed should be wholly parsed first
   * then treat the rest as a shell command. 
   * For scripting mode, the "rest" could be the "script" instruction to run
   * the contents of that file. (Different to -i in that it exits when done.) */


  return 0;  /* This should return whatever shell.c ends up with */
} 
