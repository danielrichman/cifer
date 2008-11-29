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

void keyword_bruteforce(char *intext, int intext_size, char *outtext);
void keyword_table(char *keyword, int keyword_length, int *table);
void keyword_table_preflipped(char *keyword, int keyword_length, int *table);
void keyword_table_flip(int *table);
void keyword_translate(char *intext, int text_size, char *outtext, int *table);
void keyword_table_copy(int *dest, int *source);
void keyword_single(char *intext, int intext_size, char *outtext,
                    char *keyword, int key_size, int flip);
void keyword_print_keyinfo(int *table);
void keyword_print_info(char *text, char *keyword, 
                        int *table, char *dirstring);
int keyword_check(char *keyword, int key_size);

