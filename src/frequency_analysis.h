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

typedef struct {
  int source;
  int target;
  int diff;
} rand_freq;

void random_frequency_match(int *frequency_graph,
                            int *identity_frequency_graph,
                            int *table);
void frequency_guesses(char *intext, int intext_size, 
                       char *outtext, int do_decrypt);
int frequency_analysis(char *text, int text_size, int jump);
double delta_ic (char *text, int text_size, int jump);

