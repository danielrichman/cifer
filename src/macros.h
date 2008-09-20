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

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))
#define diff(X, Y) ((X) > (Y) ? ((X) - (Y)) : ((Y) - (X)))
#define modp(X, B)  ((X) % (B))
#define modn(X, B)  (((X % B) + B) % B)

#define ASCII_CH(ch)           (ch >= 32 && ch <= 128)
#define ALPHAL_CH(ch)          (ch >= 97 && ch <= 122)
#define ALPHAH_CH(ch)          (ch >= 65 && ch <= 90)
#define NUMBER_CH(ch)          (ch >= 48 && ch <= 57)
#define ALPHANUMERIC_CH(ch)    (ALPHAL_CH(ch) || ALPHAH_CH(ch) || NUMBER_CH(ch))
#define SPACE_CH(ch)           (ch == 32)

#define ALPHA_TOUPPER(ch)      (ALPHAL_CH(ch) ? ch - 32 : ch)
#define ALPHA_TOLOWER(ch)      (ALPHAH_CH(ch) ? ch + 32 : ch)

#define IS_NEWLINE(ch)         (ch == 10)
#define CHARNUM(ch)            (ALPHAL_CH(ch) || ALPHAH_CH(ch) ? \
                                   ALPHA_TOLOWER(ch) - 97 : -1)
#define NUMCHAR(i)             (i + 97)


