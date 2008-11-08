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

/* Tries a modified version of vigenere & affine only affecting
 * non vowels that are supposed to have been already dealt with 
 * by the user; ie, converting 01234 to aeiou (these will be ignored)
 * See ncc/3b/ for an example */


// TODO: complete me and perahps rename to deform.c and add multiple
// ctext => ptext mappings

