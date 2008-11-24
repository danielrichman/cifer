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

#define DICT_INIT_CHECK   297  /* Just some random number ;) */

extern char *dict;
extern char **dict_pointer;
extern char **dict_pointer_end;

typedef struct {
  int text_size;
  int frequency_graph_tolerance;
  int num_checked;
  int num_checked_freq_ok;
  int num_checked_the_ok;
  int *frequency_graph;
  int *identity_frequency_graph;
  digram *digrams_temp;
  trigram *trigrams_temp;
  int init_check;
} score_text_pro_state;

void init_dict(void);
void unload_dict(int notice);
void load_dict(char *location);
int score_text_dict_fast(char *text, int size);
void score_text_dict_spaces(char *text, int size, int *space_array);
void score_text_pro_start(int text_size, score_text_pro_state *state);
void score_text_pro_cleanup(score_text_pro_state *state);
void score_text_pro_print_stats(char *englishname, score_text_pro_state *state);
int score_text_pro(char *text, score_text_pro_state *state);

