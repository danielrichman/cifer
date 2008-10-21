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

#define CFSH_READ_MODE_PARSECHECK   1
#define CFSH_READ_MODE_EXECUTE_SF   2
#define CFSH_READ_MODE_EXECUTE_HF   3
#define CFSH_READ_MODE_INTERACTIVE  4

#define CFSH_COMMAND_PARSEFAIL     -6
#define CFSH_COMMAND_SOFTFAIL      -7
#define CFSH_COMMAND_HARDFAIL      -8

#define CFSH_OK                     0
#define CFSH_BREAK_LOOP             99

void cfsh_init();
void cfsh_autoinit();
int cfsh_read(FILE *read, int mode);
void cfsh_scriptfile(char *name, int preparse, int softfail);
int cfsh_line(char *input, int mode);

#define cfsh_interactive()  cfsh_read(stdin, CFSH_READ_MODE_INTERACTIVE)

