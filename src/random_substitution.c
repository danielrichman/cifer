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

void crack_random_substitution(char *text, int text_size)
{
  /* Start with trigrams & common words. the, that, will, etc. */
  /* do the same for some bigrams. keep this minimal to avoid error */
  /* Fill in the remainder with the dictionary and frequency. */
  /* Perhaps some routine to fit partial-words and replace/fix a mistake 
   * using the dictionary and some bare skills */
}

