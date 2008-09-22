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
  int parent_id;
  int parent_score;
} ga_parent_list;

void monoalph_ga_crack(char *text, int text_size);
void auto_monoalph_parents(char *text, int text_size, int *target_success);
void monoalph_imp_genetic(int parents[30][26], char *text, int text_size,
                             int *target_success);

