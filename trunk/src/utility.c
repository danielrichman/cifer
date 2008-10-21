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

void *malloc_good(size_t size)
{
  void *r;

  r = malloc(size);
  if (r == NULL)
  {
    printf("\n\nmalloc_good: Unable to malloc(%i), auto-exiting\n\n",
                                                                  size);
    exit(1);
  }

  return r;
}

void *realloc_good(void *ptr, size_t size)
{
  void *r;

  r = realloc(ptr, size);
  if (size != 0 && r == NULL)
  {
    printf("\n\nrealloc_good: Unable to realloc(%i), auto-exiting\n\n",
                                                                  size);
    exit(1);
  }

  return r;
}

size_t strtlens(const char *s, size_t sz)
{
  return sz - strlefts(s, sz) - strrights(s, sz);
}

size_t strlefts(const char *s, size_t sz)
{
  int i, left;
  char ch;

  for (i = 0, left = 0; i < sz; i++)
  {
    ch = *(s + i);

    if (XSPACE_CH(ch) || SPACE_CH(ch))  left++;
    else                                break;
  }

  return left;
}

size_t strrights(const char *s, size_t sz)
{
  int i, right;
  char ch;
  
  for (i = sz - 1, right = 0; i >= 0; i--)
  {
    ch = *(s + i);

    if (XSPACE_CH(ch) || SPACE_CH(ch))  right++;
    else                                break;
  }

  return right;
}

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

#define INSERTION_DEFINE(ty, sval) void insertion_sort_ ## ty \
                                            (ty *a, int asize) { \
    int i, j, k; ty d; k = asize; for (i = 0; i < k; i++) { \
    d = a[i]; j = i - 1; while (j >= 0 && a[j].sval > d.sval) { \
    a[j + 1] = a[j]; j = j - 1; } a[j + 1] = d; } }
#define GET_DEFINE(nm, ty, sval, svalty, comp) ty nm ## _ ## ty  \
                                            (ty *a, int asize) { \
    int i, best_key; svalty best; best = a[0].sval; best_key = 0; \
    for (i = 1; i < asize; i++) \
    if (a[i].sval comp best) { best = a[i].sval; best_key = i; } \
    return a[best_key]; }
#define GET_KEY_DEFINE(nm, ty, sval, svalty, comp) int nm ## _ ## ty ## _key  \
                                            (ty *a, int asize) { \
    int i, best_key; svalty best; best = a[0].sval; best_key = 0; \
    for (i = 1; i < asize; i++) \
    if (a[i].sval comp best) { best = a[i].sval; best_key = i; } \
    return best_key; }

#include "utility.i"

#undef GET_KEY_DEFINE
#undef GET_DEFINE
#undef INSERTION_DEFINE

/* And this one is the same for this function: 
 * vigenere_colum_ic max_columnic(vigenere_column_ic a[], int asize)
 * {
 *   int i, best, best_key;
 *
 *   best = a[0];
 *   for (i = 1; i < asize; i++) if (a[i].column_ic_diff > best)
 *   { best = a[i].column_ic_diff; best_key = i; }
 *
 *   return a[best_key];
 * }
 * And min_ is just the opposite comparison */

/* a = number; b = modular */
int modular_multiplicative_inverse(int a, int mbase)
{
  int b, x, y, temp, quotient, lastx, lasty;

  b = mbase;
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

  return modn(lastx, mbase);
}

int gcd(int a, int b)
{
  if (b == 0)  return a;
  else return gcd(b, modn(a, b));
}

