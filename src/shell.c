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
  init_buffers();
}

void cfsh_autoinit()
{
  /* Automatic "we guessed you wanted this" stuff:
   *  Make 10 Buffers,
   *  Load the Dictionary */ 
}

int cfsh_interactive()
{
  /* Enter the Loop, accepting stuff from stdin and passing to cfsh_line */
}

int cfsh_scriptfile(char *name)
{
  /* Execute the file, taking each line and giving it to cfsh_line */
}

int cfsh_line(char *input)
{
  /* Use command.c to parse it, execute it and free it. command.c functions
   * won't produce output or errors to stdout, so you must do that here. */ 
}

