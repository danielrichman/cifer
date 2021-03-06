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

int affine_solve(int cl_1, int pl_1, int cl_2, int pl_2, int *a, int *b);
void crack_affine(char *intext, int intext_size, char *outtext);
void affine_bf(char *intext, int intext_size, char *outtext);
void affine_encode(char *intext, int intext_size, char *outtext, int a, int b);
void affine_decode(char *intext, int intext_size, char *outtext, int a, int b);

