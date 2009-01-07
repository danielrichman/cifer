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

#define WORD_BUF_SIZE          1024
#define MIN_WORD_SIZE          2

#define DESIGNLANG             "English"
#define OPTIMAL_DELTA_IC       1.73  /* Static data on English */ 

#define AFFINE_WAIT            1     /* How long affine waits before \
                                      * doing utility routines */

#define DEFAULT_BUFFER_SIZE    0

