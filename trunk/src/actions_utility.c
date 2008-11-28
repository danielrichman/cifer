/*
    Cifer: Automating classical cipher cracking in C
    Copyright (C) 2008  Daniel Richman & Simrun Basuita

    Cifer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cifer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cifer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdinc.h"

#define ACTION_USAGE action_mmi_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_mmi(int argc, char **argv)
{
  int value, mod, modo, result, vgcd;

  actionu_argchk(2)
  actionu_intparse(*(argv)    , value)
  actionu_intparse(*(argv + 1), mod)

  if (value < 1 || mod < 1) actionu_fail()

  vgcd = gcd(value, mod);
  modo = mod;

  if (vgcd != 1)
  {
    printf("Warning: Cannot MMI if the GCD(value, mod) is not 1. \n");
    printf("Taking factor of %i out of args.\n\n", vgcd);
  }

  printf("MMI:   %i (mod %i) \n", value, mod);

  if (vgcd != 1)
  {
    value = value / vgcd;
    mod = mod / vgcd;
    printf("GCD:   %i (mod %i) \n", value, mod);
  }

  result = modular_multiplicative_inverse(value, mod);

  printf("Answer:   %i\n", result);
  printf("So        %i * %i == %i (mod %i) \n", value, result, 
                                     modn(value * result, mod), mod);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_gcd_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_gcd(int argc, char **argv)
{
  int i, running;
  int *numbers;

  if (argc < 2) actionu_fail()

  numbers = malloc_good( sizeof(int) * argc );

  for (i = 0; i < argc; i++)
  {
    actionu_intparsef(*(argv + i), *(numbers + i), numbers)

    if (*(numbers + i) < 1)
    {
      printf(action_gcd_usage);
      return CFSH_COMMAND_HARDFAIL;
    }
  }

  printf("GCD of %i", *(numbers));
  running = *(numbers);

  for (i = 1; i < argc; i++)
  {
    printf(", %i", *(numbers + i));
    running = gcd(running, *(numbers + i));
  }

  printf(" is %i \n", running);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_coprime_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_coprime(int argc, char **argv)
{
  int a, b, r;

  actionu_argchk(2)
  actionu_intparse(*(argv)    , a)
  actionu_intparse(*(argv + 1), b)

  if (a < 1 || b < 1) actionu_fail()

  r = gcd(a, b);

  if (r == 1)  printf("%i and %i are coprime. \n", a, b);
  else         printf("%i and %i are not coprime (gcd %i) \n", a, b, r);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_charinfo_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_charinfo(int argc, char **argv)
{
  char ch;
  int num;

  actionu_argchk(1)
  if (strlen(*argv) == 0) actionu_fail()

  if (NUMBER_CH(**argv))
  {
    actionu_intparse(*argv, num)

    if (num > 26 || num < 0)
    {
      printf("mod(%i, 26) = %i \n", num, modn(num, 26));
      num = modn(num, 26);
    }

    ch = ALPHA_TOLOWER(NUMCHAR(num));
    printf("The number %i translates to the character %c%c \n", num,
            ALPHA_TOUPPER(ch), ch);
  }
  else if (strlen(*argv) == 1 && ALPHA_CH(**argv))
  {
    ch = ALPHA_TOLOWER(**argv);
    num = CHARNUM(ch);
    printf("The character %c%c is also number %i in the 0-26 alphabet.\n", 
            ALPHA_TOUPPER(ch), ch, num);
  }
  else
  {
    actionu_faili()
  }

  /* Not sure if this is system spesific, or is there a nicer
   * way to do this? */
  #define binp(a, n)  ( ((a) & (n)) == 0 ? '0' : '1' )
  #define binap(a) binp(a, 128), binp(a, 64), binp(a, 32), binp(a, 16), \
                   binp(a, 8),   binp(a, 4),  binp(a, 2),  binp(a, 1)

  /* BEWM! */ 
  printf("The character %c (lowercase) is ASCII %3i (decimal) "
             "%2x (hex) %c%c%c%c %c%c%c%c (binary)\n",
          ch, ch, ch, binap(ch));

  /* A little shorter */
  #define uch (ALPHA_TOUPPER(ch))

  /* Hopefully all the ALPHA_TOUPPER spam will be condensed by the
   * Optimiser */
  printf("The character %c (uppercase) is ASCII %3i (decimal) "
             "%2x (hex) %c%c%c%c %c%c%c%c (binary)\n\n",
          uch, uch, uch, binap(uch));

  printf("In English, the character %c has frequency: \n", ch);
  printf("    decimal probability:  %f     \n",   english_frequency[num] );
  printf("    percentage:           %.2f%% \n\n", english_frequency[num] * 100);

  printf("Bacon    for %c: %s \n",    ch, bacon_alphabet[num]);
  printf("Polybius for %c: %2i \n\n", ch, polybius_grid_25_int[num]);

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

