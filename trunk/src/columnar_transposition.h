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

typedef void(*columnar_transposition_function)(char *intext, int text_size,
                           char *outtext, int *key, int key_size);

void columnar_transposition_col2col(char *intext, int text_size,
                           char *outtext, int *key, int key_size);
void columnar_transposition_col2row(char *intext, int text_size,
                           char *outtext, int *key, int key_size);
void columnar_transposition_row2col(char *intext, int text_size,
                           char *outtext, int *key, int key_size);

void columnar_transposition_bruteforce(char *intext, int intext_size,
         char *outtext, int key_min, int key_max, 
         columnar_transposition_function routine);

void columnar_transposition_flip_key(int *key, int key_size);
void columnar_transposition_keyinfo(int *key, int key_size);
void columnar_transposition_text2key(char *text, int text_size, 
                                     int **key, int *new_key_size);

