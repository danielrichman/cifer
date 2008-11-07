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
#define modn(X, B)  ((X) % (B)) + (((X) % (B)) < 0 ? (B) : 0)

#define ASCII_CH(ch)           (ch >= 32 && ch <= 128)
#define ALPHAL_CH(ch)          (ch >= 97 && ch <= 122)
#define ALPHAH_CH(ch)          (ch >= 65 && ch <= 90)
#define NUMBER_CH(ch)          (ch >= 48 && ch <= 57)
#define ALPHA_CH(ch)           (ALPHAL_CH(ch) || ALPHAH_CH(ch))
#define ALPHANUMERIC_CH(ch)    (ALPHA_CH(ch)  || NUMBER_CH(ch))
#define SPACE_CH(ch)           (ch == 32)
#define XSPACE_CH(ch)          (ch < 32 || ch == 127)
#define QUOTE_CH(ch)           (ch == 39 || ch == 34)
#define ESCAPE_CH(ch)          (ch == 92)

#define ALPHA_TOUPPER(ch)      (ALPHAL_CH(ch) ? ch - 32 : ch)
#define ALPHA_TOLOWER(ch)      (ALPHAH_CH(ch) ? ch + 32 : ch)
#define ALPHA_FLIP_CASE(ch)    ( ALPHAL_CH(ch) ? ch - 32 :     \
                                (ALPHAH_CH(ch) ? ch + 32 : ch) )
#define ALPHA_CASEBACON(ch)    (ALPHAH_CH(ch) ? 'A' :       \
                                (ALPHAL_CH(ch) ? 'B' : ch)  )

#define IS_NEWLINE(ch)         (ch == 10)
#define CHARNUM(ch)            (ALPHAL_CH(ch) ? ch - 97 :      \
                                (ALPHAH_CH(ch) ? ch - 65 : -1) )
#define NUMCHAR(i)             (i + 97)

#define NUMCHARNUM(ch)         (NUMBER_CH(ch) ? ch - 48 : -1)
#define NUMNUMCHAR(i)          (i + 48)

