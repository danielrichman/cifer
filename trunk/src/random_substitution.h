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
  int digram_num;
  int digram_value;
} digram;

typedef struct { 
  int ptext_char;
  int ptext_char_score;
} subst_hint_ptext_item;

typedef struct {
  int ctext_char;
  subst_hint_ptext_item ptext_char_scores[26];
} subst_hint;

void crack_random_substitution(char *text, int text_size);

