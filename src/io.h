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

#define CFSH_IO_MODE_AUTO        0  /* Create the file if it doesnt exist */
#define CFSH_IO_MODE_OVERWRITE   1  /* Overwrite or create */
#define CFSH_IO_MODE_APPEND      2  /* Append or create */

#define BUFFER_FILTER_NONE       4    /* No filter applied yet */
#define BUFFER_FILTER_ALPHA      5    /* Alpha only, case preserved */
#define BUFFER_FILTER_ALPHANUM   6    /* Alphanumeric only */
#define BUFFER_FILTER_LALPHA     7    /* Alpha only, All lower */
#define BUFFER_FILTER_UALPHA     8    /* Alpha only, All upper */
#define BUFFER_FILTER_FLIPCASE   9    /* Flip Case */
#define BUFFER_FILTER_CASEBACON  10   /* Changes case to ab */
#define BUFFER_FILTER_NUM        11   /* Numbers only */
#define BUFFER_FILTER_ESP        12   /* All but spaces & xsp chars */
#define BUFFER_FILTER_ENL        13   /* All but xsp chars (see macros) */

extern char **cfsh_buffers;
extern int *cfsh_buffer_filters;
extern int *cfsh_buffer_sizes;
extern int cfsh_num_buffers;

void buffers_init();
void create_buffers(int num);
void size_buffer_array(int num)

int file2buffer(char *name, int buffer_id);
int buffer2file(char *name, int buffer_id, int mode);

void initbuffer(int buffer_id);
void destroybuffer(int buffer_id);
void resisebuffer(int buffer_id, int newsize);
void copybuffer(int buffer_id_1, int buffer_id_2);
void filterbuffer(int buffer_id, int mode);
void clearbuffer(int buffer_id);

#define get_buffer(i)        (*(cfsh_buffer + i))
#define get_buffer_filter(i) (*(cfsh_buffer_filters + i))
#define get_buffer_size(i)   (*(cfsh_buffer_sizes + i))
#define get_buffer_real_size(i) (strnlen(get_buffer(i), get_buffer_size(i)))
#define setbuffernull(i) \
          (*(*(cfsh_buffer_sizes + i) + *(cfsh_buffer_size + i)) = 0)
