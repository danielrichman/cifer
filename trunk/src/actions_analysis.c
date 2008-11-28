/*
    Cifer: Automating classical cipher cracking in C
    Copyright (C) 2008  Daniel Richman & Simrun Basuita

    Cifer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cifer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cifer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdinc.h"

#define ACTION_USAGE action_deltaic_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_deltaic(int argc, char **argv)
{
  int buffer_id;
  double result;

  actionu_argchk(1)
  actionu_bufferparse(*(argv), buffer_id)

  result = delta_ic(get_buffer(buffer_id), get_buffer_real_size(buffer_id), 1);
  printf("The DELTA_IC of buffer_%i is %f\n", buffer_id, result);
  printf("A typical value for English is %f. buffer_%i differs by %f.\n",
               OPTIMAL_DELTA_IC, buffer_id, diff(result, OPTIMAL_DELTA_IC));

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_fg_usage
#define ACTION_FAIL CFSH_COMMAND_HARDFAIL
int action_fg(int argc, char **argv)
{
  int buffer_in, buffer_out;
  char *outtext;

  if (argc != 1 && argc != 2) actionu_fail()

  actionu_bufferparse(*(argv),       buffer_in)
  outtext = NULL;

  if (argc == 2)
  {
    actionu_bufferparse(*(argv + 1), buffer_out)
    actionu_bufferchk(buffer_in, buffer_out)
    actionu_bufferschk(buffer_in, buffer_out);
    outtext = get_buffer(buffer_out);
  }

  frequency_guesses(get_buffer(buffer_in), get_buffer_real_size(buffer_in), 
                    outtext, 1);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_ifg_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_ifg(int argc, char **argv)
{
  int buffer_id, text_size, i, m;
  int ifg[26], width[26];

  actionu_argchk(1)

  if (**argv == 'b')
  {
    actionu_bufferparse(*argv, buffer_id)
    text_size = get_buffer_real_size(buffer_id);
  }
  else
  {
    actionu_intparse(*argv, text_size)
  }

  if (text_size < 0) text_size = 0;

  create_identity_frequency_graph(ifg, text_size);
  printf("Printing identity frequency graph for text_size %i\n", text_size);

  print_setup_width(width, &m);
  print_count_width(ifg, width, &m);
  print_finalise_width(width, &m);

  printf("C|");
  for (i = 0; i < 26; i++) printf("%*c|", width[i], NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], i);
  printf("\n");

  printf("F|");
  for (i = 0; i < 26; i++) printf("%*i|", width[i], ifg[i]);
  printf("\n\n");

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_fa_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_fa(int argc, char **argv)
{
  int buffer_id;
  actionu_argchk(1)
  actionu_bufferparse(*(argv),     buffer_id)
  frequency_guesses(get_buffer(buffer_id), get_buffer_real_size(buffer_id), 
                    NULL, 0);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_digrams_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_digrams(int argc, char **argv)
{
  int buffer_id, num_to_show, i;
  digram *tgt;

  actionu_argchk(2)
  actionu_bufferparse(*(argv),  buffer_id)
  actionu_intparse(*(argv + 1), num_to_show)

  if (get_buffer_real_size(buffer_id) < 2)
  {
    printf("Input buffer text too small\n");
    actionu_faili()
  }

  if (num_to_show < 1)          num_to_show = 1;
  if (num_to_show > (26 * 26))  num_to_show = (26 * 26);
  tgt = malloc_good( sizeof(digram) * num_to_show );
  count_digrams(get_buffer(buffer_id), get_buffer_real_size(buffer_id),
                tgt, num_to_show);

  printf("Showing top %i digram%c..%c\n", num_to_show,
                (num_to_show == 1 ? '.' : 's'),
                (num_to_show == 1 ? ' ' : '.'));

  for (i = 0; i < num_to_show; i++)
  {
    printf("Rank %i)  \t %c%c\n", i, NUMCHAR(tgt[i].digram_ch1),
                                     NUMCHAR(tgt[i].digram_ch2)  );
  }

  printf("\n");

  free(tgt);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_trigrams_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_trigrams(int argc, char **argv)
{
  int buffer_id, num_to_show, i;
  trigram *tgt;

  actionu_argchk(2)
  actionu_bufferparse(*(argv),  buffer_id)
  actionu_intparse(*(argv + 1), num_to_show)

  if (get_buffer_real_size(buffer_id) < 3)
  {
    printf("Input buffer text too small\n");
    printf(action_trigrams_usage);
    return CFSH_COMMAND_HARDFAIL;
  }

  if (num_to_show < 1)          num_to_show = 1;
  if (num_to_show > (26 * 26))  num_to_show = (26 * 26);
  tgt = malloc_good( sizeof(trigram) * num_to_show );
  count_trigrams(get_buffer(buffer_id), get_buffer_real_size(buffer_id),
                tgt, num_to_show);

  printf("Showing top %i trigram%c..%c\n", num_to_show,
                (num_to_show == 1 ? '.' : 's'),
                (num_to_show == 1 ? ' ' : '.'));

  for (i = 0; i < num_to_show; i++)
  {
    printf("Rank %i)  \t %c%c%c\n", i, NUMCHAR(tgt[i].trigram_ch1),
                                       NUMCHAR(tgt[i].trigram_ch2),
                                       NUMCHAR(tgt[i].trigram_ch3)  );
  }

  printf("\n");

  free(tgt);
  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

#define ACTION_USAGE action_pct_usage
#define ACTION_FAIL CFSH_COMMAND_SOFTFAIL
int action_pct(int argc, char **argv)
{
  int i;

  printf("C|");
  for (i = 0; i < 26; i++) printf("%2c|", NUMCHAR(i));
  printf("\n");

  printf("N|");
  for (i = 0; i < 26; i++) printf("%2i|", i);
  printf("\n\n");

  return CFSH_OK;
}
#undef ACTION_USAGE
#undef ACTION_FAIL

