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
  digram  digrams[10];
  trigram trigrams[1];

  /* Count digrams & trigrams */
  count_digrams(text, text_size, digrams, 10);
  count_trigrams(text, text_size, trigrams, 1);

  /* See if we can find the right digram, just to double check */
  j = 0;

  for (i = 0; i < 10; i++)
  {
    if (digrams[i].digram_ch1 == trigrams[0].trigram_ch1 &&
        digrams[i].digram_ch2 == trigrams[0].trigram_ch2)
    {
      j = i;
    }
  }

  /* Make sure that the first match for each is TH and THE */
  if (digrams[j].digram_ch1 != trigrams[0].trigram_ch1 || 
      digrams[j].digram_ch2 != trigrams[0].trigram_ch2)
  {
    printf("\ncrack_affine: Ambiguous failure: Unable to find THE\n");
    printf("Affine best digram %c%c: %i, trigram %c%c%c: %i.\n",
          NUMCHAR(digrams[j].digram_ch1), NUMCHAR(digrams[j].digram_ch2),
          digrams[j].digram_value,
          NUMCHAR(trigrams[0].trigram_ch1), NUMCHAR(trigrams[0].trigram_ch2),
          NUMCHAR(trigrams[0].trigram_ch3),
          trigrams[0].trigram_value);

    return;
  }

  /* Let the ctext for T be x, and H be y, and the plaintext t is 19,
   * plaintext H is 7. Unknowns are a and b */
  /* 19a + b = x      7a + b = y */

  /* CTEXT for T is in digrams[j].digram_ch1 as a CHARNUM */
  /* CTEXT for H is in digrams[j].digram_ch2 as a CHARNUM */

  printf("Affine best digram %c%c: %i, trigram %c%c%c: %i, let TH == %c%c\n",
        NUMCHAR(digrams[j].digram_ch1), NUMCHAR(digrams[j].digram_ch2),
        digrams[j].digram_value,
        NUMCHAR(trigrams[0].trigram_ch1), NUMCHAR(trigrams[0].trigram_ch2),
        NUMCHAR(trigrams[0].trigram_ch3), 
        trigrams[0].trigram_value,
        ALPHA_TOUPPER(NUMCHAR(digrams[j].digram_ch1)), 
        ALPHA_TOUPPER(NUMCHAR(digrams[j].digram_ch2)) );

  affine_solve(digrams[j].digram_ch1, CHARNUM('T'), 
               digrams[j].digram_ch2, CHARNUM('H'), 
               &a, &b);

  /* Fancy way of decoding: find the mod mul. inverse */
  j = modular_multiplicative_inverse(a, 26);

  printf("Affine: Finding modular multiplicative inverse(a, 26) = %i;\n", j);

  for (i = 0; i < text_size; i++)
  {
    /* Subtract b, multiply by the inverse and mod */
    *(text + i) = NUMCHAR( modn((CHARNUM(*(text + i)) - b) * j, 26) );
  }

  /* Print the results */
  printf("Affine: Deciphered Plaintext (hopefully):\n\n");
  printf("%*s\n\n", text_size, text);
}

int affine_solve(int cl_1, int pl_1, int cl_2, int pl_2, int *a, int *b)
{
  printf("Solving affine equation: CL => PL: %d => %d (%c), %d => %d (%c)\n",
                        cl_1, pl_1, NUMCHAR(pl_1), cl_2, pl_2, NUMCHAR(pl_2));
  
  *a = (modn((cl_1 - cl_2), 26)) / (pl_1 - pl_2);
  *b = ((modn(cl_1, 26)) - (*a * pl_1)) / *a;

  printf("             Affine solution: a = %i, b = %i\n", *a, *b);

  if (IS_COPRIME(*a, 26))
  {
    printf("             Affine solution seems OK (a coprime 26)\n");
    return 0;
  }
  else
  {
    printf("             Affine solution might be bad (a not coprime 26)\n");
    return -1;
  }
}
