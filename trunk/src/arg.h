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

extern char *arg_input;
extern char *arg_output;
extern int arg_freq;
extern int arg_pct;
extern int arg_vig;
extern int arg_aff;
extern int arg_aff_bf;
extern int arg_gama;
extern int arg_keyb;
extern int arg_keyd;
extern char *arg_keyd_string;
extern int arg_ctrans;
extern columnar_transposition_function arg_ctrans_type;
extern int arg_ctrans_mode;
extern int *arg_ctrans_key;
extern int arg_ctrans_key_size;
extern int arg_manual_shift;
extern int arg_manual_shift_size;
extern int *arg_manual_shift_key;
extern int arg_gcd;
extern int arg_gcd_1;
extern int arg_gcd_2;
extern int arg_mmi;
extern int arg_mmi_1;
extern int arg_mmi_2;
extern int arg_pt;
extern int arg_pt_1;
extern int arg_pd;
extern int arg_pd_1;
extern int arg_fd;
extern int arg_is;
extern int arg_is_1;
extern int arg_bacon;
extern int arg_pb;
extern int arg_rfbf;
extern int arg_rfbfmin;
extern int arg_rfbfmax;

/* Parse cmd-line args into variables. Returns 0 if OK, -1 if error */
int arg_parse(int argc, char **argv);
