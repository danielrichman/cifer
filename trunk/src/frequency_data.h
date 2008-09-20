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

extern int *ch_freq_ar;

typedef struct {
  int digram_ch1;
  int digram_ch2;
  int digram_value;
} digram;

typedef struct {
  int trigram_ch1;
  int trigram_ch2;
  int trigram_ch3;
  int trigram_value;
} trigram;

void create_identity_frequency_graph(int frequency_graph[], int text_size);

/* Returns an int array[26] with letter frequencies of input text */
void count_freq(char *text, int input_size, int tgt[]);
void count_digrams(char *text, int input_size, digram tgt[], int tgt_size);
void count_trigrams(char *text, int input_size, trigram tgt[], int tgt_size);

