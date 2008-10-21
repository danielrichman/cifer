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

/*  typedef int(*cfsh_command)(int argc, char **argv);
 *
 *     CFSH_OK                     0
 *     CFSH_BREAK_LOOP             99 */

#define ACTIONU_BUFFERPARSE_FAIL  -1
int actionu_bufferparse(char *str, const char *u);

int action_buffers(int argc, char **argv);
int action_quit(int argc, char **argv);
int action_resize(int argc, char **argv);
int action_clear(int argc, char **argv);
int action_copy(int argc, char **argv);
int action_load(int argc, char **argv);
int action_write(int argc, char **argv);
int action_filter(int argc, char **argv);
int action_getsize(int argc, char **argv);
int action_getfilter(int argc, char **argv);
int action_nullbuffer(int argc, char **argv);
int action_script(int argc, char **argv);
int action_quickscript(int argc, char **argv);
int action_softscript(int argc, char **argv);
int action_quicksoftscript(int argc, char **argv);
int action_score(int argc, char **argv);
int action_loaddict(int argc, char **argv);
int action_affine(int argc, char **argv);
int action_affinesolve(int argc, char **argv);
int action_affinebf(int argc, char **argv);
int action_bacon(int argc, char **argv);
int action_shift(int argc, char **argv);
int action_deltaic(int argc, char **argv);
int action_monoalph(int argc, char **argv);
int action_ctrans(int argc, char **argv);
int action_fg(int argc, char **argv);
int action_ifg(int argc, char **argv);
int action_fa(int argc, char **argv);
int action_digrams(int argc, char **argv);
int action_trigrams(int argc, char **argv);
int action_pct(int argc, char **argv);
int action_keyb(int argc, char **argv);
int action_keyt(int argc, char **argv);
int action_keytf(int argc, char **argv);
int action_keyd(int argc, char **argv);
int action_polybius(int argc, char **argv);
int action_rfbf(int argc, char **argv);
int action_vigenere(int argc, char **argv);
int action_mmi(int argc, char **argv);
int action_gcd(int argc, char **argv);
int action_coprime(int argc, char **argv);
int action_charinfo(int argc, char **argv);

/* int action_(int argc, char **argv); */

