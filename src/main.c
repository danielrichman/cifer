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

int main(int argc, char **argv)
{
  int table[26];
  score_text_pro_state pro_state;

  if ((arg_parse(argc, argv)) == -1) /* Bad argument(s) */
  {
    printf("Bad argument(s), exiting!\n");
    return 0;
  }
  
  action_do();
  load_dict();

  printf("\n\nBeefburger keyword score test debug on intext:...\n\n");
  keyword_table_preflipped("beefburger", 10, table);
  keyword_decode(intext, intext, intext_size, table);
  printf("%s\n\n", intext);
  score_text_pro_start(intext_size, &pro_state);
  printf("score: %i\n", score_text_pro(intext, &pro_state));
  score_text_pro_print_stats("main_test", &pro_state);
  score_text_pro_cleanup(&pro_state);

  return 0;
} 

