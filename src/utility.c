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

/* The following macro is a condensed version of this code 
 * void insertion_columnic_sort(vigenere_column_ic a[], int asize)
 * {
 *   int i, j, k;
 *   vigenere_column_ic d;
 *     
 *   k = asize - 1;
 *   for (i = 0; i < k; i++)
 *   {
 *     d = a[i];
 *     j = i - 1;
 *
 *     while (j >= 0 && a[j].column_ic_diff > d.column_ic_diff)
 *     {
 *       a[j + 1] = a[j];
 *       j = j - 1;
 *     }
 *
 *     a[j + 1] = d;
 *   }
 * }                                                                */

#define INSERTION_DEFINE(nm, ty, sval) void nm(ty a[], int asize) { \
    int i, j, k; ty d; k = asize; for (i = 0; i < k; i++) { \
    d = a[i]; j = i - 1; while (j >= 0 && a[j].sval > d.sval) { \
    a[j + 1] = a[j]; j = j - 1; } a[j + 1] = d; } }

#include "utility.i"
#undef INSERTION_DEFINE

int modular_multiplicative_inverse(int a, int b)
{
  int x, y, temp, quotient, lastx, lasty;

  x = 0;     y = 1;
  lastx = 1; lasty = 0;

  while (b != 0)
  {
    temp = b;
    quotient = a / b;
    b = modn(a, b);
    a = temp;

    temp = x;
    x = lastx - (quotient * x);
    lastx = temp;

    temp = y;
    y = lasty - (quotient * y);
    lasty = temp;
  }

  if (lasty < 0) return lasty;
  if (lastx < 0) return lastx;

  return 0; /* Fail */
}

int gcd(int a, int b)
{
  if (b == 0)  return a;
  else return gcd(b, modn(a, b));
}

/* http://en.wikipedia.org/wiki/Method_of_successive_substitution */
/* For any equation ax + b = c (mod d) */

void modular_sucessive_substitution(int &result_x, &result_mod,
           int eq1_xcoeff, int eq1_coeff, int eq1_mod, int eq1_result,
           int eq2_xcoeff, int eq2_coeff, int eq2_mod, int eq2_result)
{
  int p, r, u, v, w, n, o;

  /* prepare the return variables */
  *result_x = 0;
  *result_mod = 0;

  /* modular arithmetic permits subtraction from both sides */
  eq1_result -= eq1_coeff;
  eq2_result -= eq2_coeff;

  /* fix the now possibly damaged results with modn */
  eq1_result = modn(eq1_result, eq1_mod);
  eq2_result = modn(eq2_result, eq2_mod);

  /* This is an expanded version of the below comments. enjoy! */

  p = modn(eq1_result - eq1_coeff, eq1_mod);
  n = gcd(eq1_xcoeff, gcd(p, eq1_mod));
  r = modn(eq2_result - eq2_coeff, eq2_mod);
  o = gcd(eq2_xcoeff, gcd(r, eq2_mod));
  w = eq2_mod / o;
  v = (modular_multiplicative_inverse(eq2_xcoeff / o, w) * r) - 
      (modular_multiplicative_inverse(eq1_xcoeff / n, eq1_mod / n) * p)
  u = gcd(eq1_mod, v, w);

  *result_x   = (modular_multiplicative_inverse(eq1_xcoeff/n,eq1_mod/n)*p) + 
                ((eq1_mod * (v/u)) * 
                 modular_multiplicative_inverse(eq1_mod/u,w/u));
  *result_mod = eq1_mod * (w/u);

  /* Definitions:
   *    a = eq1_xcoeff
   *    b = eq1_coeff
   *    c = eq1_result
   *    d = eq1_mod
   *    e = eq2_xcoeff
   *    f = eq2_coeff
   *    g = eq2_result
   *    h = eq2_mod
   *    s is a function, modn, that fixes a negative modulus
   *    gcd is the GCD Function (although for three variables it may have
   *                           to be applied as gdc(a, gdc(b, c))  )
   *    m is a function, that finds the multiplactive modular inverse
   *    p, r, u, v, w, n, o are storage variables.*/

  /* To tackle this, we'll represent each variable with some 
   * tasty short letters, solve it using algebraic rearranging, then
   * bring back the variables */
  /* To simplify a coeffecient, we can just subtract, that's ok with mods.
   * to simplify an x_coefficient, we have to use the modular multiplicative
   * inverse and multiply both sides by that. */
  /*  (1)  ax + b = c (mod d)
   *  (2)  ex + f = g (mod h)
   *
   *  Simplify (1) =>  ax = s(c-b, d) (mod d)
   *           STO =>  p = s(c-b,d)
   *               =>  ax = p (mod d)
   *               $   find the GCD of (a, p, d) and divide everything
   *                   by it. store the result in n
   *           STO =>  n = gcd(a, p, d);
   *               =>  (a/n)x = p (mod d/n)
   *               $  find mod-mul-inverse of a/n (function shorthand m)
   *               Note that a * m(a/n,d/n) = 1, or the unit (can be discarded)
   *               =>  x = p * m(a/n,d/n) (mod d/n)
   *  Simplify (2) =>  x = r * m(e/o,h/o) (mod h/o)
   *           STO =>  r = s(g-f,h)
   *           STO =>  o = gcd(e, r, h);
   *
   *  Since that the large ugly part evaluates constant, we now have an equ.
   *  in form  x = z (mod d) - that's good! but things are about to get 
   *  messy.
   *
   *  For x = z (mod y) to be true, x = z + y*j for some integer j.
   *  Using that logic in (1)...
   *               =>  x = m(a/n,d/n)p + dj
   *  Substitute that into (2)...
   *               =>  m(a/n,d/n)p + dj = m(e/o,h/o)r (mod h/o)
   *
   *  We want this in form dj = constant (mod h) so we subtract the constant
   *  part from each side, and possibly fix the modn...
   *               => dj = m(e/o,h/o)r - m(a/n,d/n)p (mod h/o)
   *
   *  Simplify the equation for better results! Find the GCD and remove it...
   *           STO => w = h/o
   *           STO => v = m(e/o,w)r - m(a/n,d/n)p
   *           STO => u = gcd(d, v, w)
   *               => (d/u)j = v/u (mod w/u)
   *
   *  Now to get rid of the d we use the tasty mod mul inverse (m())
   *               => j = v/u * m(d/u, w/u)  (mod w/u)
   *
   *  Back to the `for some integer` rule, so we can say that:
   *               => j = (v/u)m(d/u, w/u) + (w/u)k
   *                    for some integer k.
   *
   *  Substitute back into `x = m(a/n,d/n)p + dj`
   *               => x = m(a/n,d/n)p + d((v/u)m(d/u, w/u) + (w/u)k)
   *               => x = m(a/n,d/n)p + d(v/u)m(d/u, w/u) + d(w/u)k
   * 
   *  Using the reverse of the logic that created k we can turn it back into
   *  a (mod dh) form...
   *               => x = m(a/n,d/n)p + d(v/u)m(d/u, w/u) (mod d(w/u))
   *
   *  So we can now compute result_x and result_mod:
   *  1) Make all the STO variables in order.
   *  2) Use the final line to create result_x and result_mod: */
}
