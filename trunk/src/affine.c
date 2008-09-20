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

void crack_affine(char *text, int text_size)
{
  int a, b, i, j;
  digram  digrams[1];
  trigram trigrams[1];

  /* Count digrams & trigrams */
  count_digrams(text, text_size, digrams, 1);
  count_trigrams(text, text_size, trigrams, 1);

  /* Make sure that the first match for each is TH and THE */
  if (digrams[0].digram_ch1 != trigrams[0].trigram_ch1 || 
      digrams[0].digram_ch2 != trigrams[0].trigram_ch2)
  {
    printf("\ncrack_affine: Ambiguous failure: Unable to find THE\n");
    return;
  }

  /* Let the ctext for T be x, and H be y, and the plaintext t is 19,
   * plaintext H is 7. Unknowns are a and b */
  /* 19a + b = x      7a + b = y */

  /* CTEXT for T is in digrams[0].digram_ch1 as a CHARNUM */
  /* CTEXT for H is in digrams[0].digram_ch2 as a CHARNUM */

  /* TODO HERE: solve the equation and put teh results of A and B into
   * the variables M and C */

  /* END OF TODO */

  /* Fancy way of decoding: find the mod mul. inverse */
  j = modular_multiplicative_inverse(a, 26);

  for (i = 0; i < text_size; i++)
  {
    /* Subtract b, multiply by the inverse and mod */
    *(text + i) = NUMCHAR( MMIM_POS(CHARNUM(*(text + i)) - b, j, 26) );
  }
}

