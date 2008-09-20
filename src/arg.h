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

extern char *arg_input;     /* !NULL = load this file as input */
extern int arg_freq;        /* Do frequency analysis */
extern int arg_pct;         /* Print char->number table */
extern int arg_vig;         /* Do a vigenere crack */
extern int arg_aff;         /* Do an affine crack */
extern int arg_gcd;         /* Do a GCD Calculation */
extern int arg_gcd_1;
extern int arg_gcd_2;
extern int arg_pct;         /* Print trigram frequencies */

/* Parse cmd-line args into variables. Returns 0 if OK, -1 if error */
int arg_parse(int argc, char **argv);
