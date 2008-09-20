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

#define INSERTION_DEFINE(nm, ty, sval) void nm(ty a[], int asize);
#include "utility.i"
#undef INSERTION_DEFINE

#define IS_COPRIME(a, b)         (gcd((a), (b)) == 1 ? 1 : 0)

int modular_multiplicative_inverse(int a, int b);
int gcd(int a, int b);
void modular_sucessive_substitution(int &result_x, &result_mod,
           int eq1_xcoeff, int eq1_coeff, int eq1_mod, int eq1_result,
           int eq2_xcoeff, int eq2_coeff, int eq2_mod, int eq2_result);

