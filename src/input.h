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

#define INPUT_ERR_FOPEN     -1
#define INPUT_ERR_ZERO_SIZE -2
#define INPUT_ERR_MALLOC    -3

/* Open our input source(s). Returns 0 if all OK,
 *                                   INPUT_ERR_FOPEN if fopen failed,
 *                                   INPUT_ERR_ZERO_SIZE if the input file is
 *                                                       0 bytes long
 *                                   INPUT_ERR_MALLOC if malloc failed */
int input_open(char *filename);

/* Counts the size of infile. Returns the size (in bytes) */
int input_file_size(FILE *infile);

/* Shifts intext by 'shift' amount */
void shift_text(char *intext, int intext_size, int shift);

/* Restores to the original - without using fopen :) */
void input_restore();

/* Fairly self-explanatry */
void input_flip_case();
