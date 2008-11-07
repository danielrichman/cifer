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

void monoalph_substitute(char *intext, int intext_size, char *outtext,
                                             int *translation)
{
  int i;
  char ch;

  for (i = 0; i < intext_size; i++)
  {
    ch = *(intext + i);
    *(outtext + i) = ALPHA_CH(ch) ? NUMCHAR(translation[CHARNUM(ch)]) : ch;
  }
}

void caesar_cipher_enc(char *intext, int intext_size, char *outtext,
                                             int *shift, int shift_size)
{
  int i;
  char ch;

  for (i = 0; i < intext_size; i++)
  {
    ch = *(intext + i);
    *(outtext + i) = ALPHA_CH(ch) ? NUMCHAR( modn( CHARNUM(ch) +
                                 *(shift + modp(i, shift_size)), 26) ) : ch;
  }
}

void caesar_cipher_dec(char *intext, int intext_size, char *outtext,
                                             int *shift, int shift_size)
{
  int i;
  char ch;

  for (i = 0; i < intext_size; i++)
  {
    ch = *(intext + i);
    *(outtext + i) = ALPHA_CH(ch) ? NUMCHAR( modn( CHARNUM(ch) -
                             *(shift + modp(i, shift_size)), 26) ) : ch;
  }
}

