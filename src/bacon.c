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

char *bacon_alphabet[26] = { "AAAAA", "AAAAB", "AAABA", "AAABB", 
                             "AABAA", "AABAB", "AABBA", "AABBB", 
                             "ABAAA", "ABAAA", "ABAAB", "ABABA", 
                             "ABABB", "ABBAA", "ABBAB", "ABBBA", 
                             "ABBBB", "BAAAA", "BAAAB", "BAABA", 
                             "BAABB", "BAABB", "BABAA", "BABAB", 
                             "BABBA", "BABBB" };

int bacon_crack(char *intext, int intext_size)
{
  return 0;
}
