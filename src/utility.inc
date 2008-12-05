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

INSERTION_DEFINE(vigenere_column_ic, column_ic_diff)
INSERTION_DEFINE(digram,             digram_value)
INSERTION_DEFINE(trigram,            trigram_value)
INSERTION_DEFINE(rand_freq,          diff)

GET_DEFINE(best, digram,  digram_value, int, >)
GET_DEFINE(best, trigram, trigram_value, int, >)

GET_KEY_DEFINE(best, digram,  digram_value, int, >)
GET_KEY_DEFINE(best, trigram, trigram_value, int, >)

