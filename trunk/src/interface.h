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

void cc_setup(int argc, char **argv);

void print_digrams(digram input[], int size);
void print_trigrams(trigram input[], int size);

/* Format and print letter frequencies. Takes an int array[size] of
 * frequencies */
void print_freq(int freq[], int idfreq[]);

void print_setup_width(int width[], int *m);
void print_count_width(int freq[], int width[], int *m);
void print_finalise_width(int width[], int *m);

/* Print a table of letters mapped to their numeric equivalents */
void print_char_table(void);

extern char *intext;
extern int intext_size;
