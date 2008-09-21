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

/* This function fills out an int array[26] from the keyword */
void keyword_table(char *keyword, int keyword_length, int *table);

/* This function will take a keyword and decode a keyword cipher */
void keyword_decode(char *text, int text_size, char *keyword, int key_size);

/* This takes a array[plaintext]  => ciphertext and makes it into
 *              array[ciphertext] => plaintext */
void keyword_table_flip(int *table);

