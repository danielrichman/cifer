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

#define INSERTION_DEFINE(ty, sval) void insertion_sort_ ## ty \
                                            (ty *a, int asize);
#define GET_DEFINE(nm, ty, sval, svalty, comp) ty nm ## _ ## ty  \
                                            (ty *a, int asize);
#define GET_KEY_DEFINE(nm, ty, sval, svalty, comp) int nm ## _ ## ty ## _key  \
                                            (ty *a, int asize);

#include "utility.i"

#undef GET_KEY_DEFINE
#undef GET_DEFINE
#undef INSERTION_DEFINE

#define IS_COPRIME(a, b)         (gcd((a), (b)) == 1 ? 1 : 0)

int modular_multiplicative_inverse(int a, int b);
int gcd(int a, int b);
void *malloc_good(size_t size);
void *realloc_good(void *ptr, size_t size);
size_t strtlen(const char *s);

