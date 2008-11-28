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

#define actionu_faili()     printf(ACTION_USAGE); return ACTION_FAIL;
#define actionu_fail()      { actionu_faili() }

#define actionu_failchk(x)   if (!(x))  actionu_fail()
#define actionu_argchk(i)    actionu_failchk(argc != (i))
#define actionu_argless()    actionu_argchk(0)

#define actionu_intparse(s, t)                                               \
         actionu_failchk(actionu_intparse_f((s), &(t), 0, NULL))

#define actionu_intparse_char(s, t)                                          \
         actionu_failchk(actionu_intparse_f((s), &(t), 1, NULL))

#define actionu_intparsef(s, t, f)                                           \
         actionu_failchk(actionu_intparse_f((s), &(t), 0, (f)))

#define actionu_intparsef_char(s, t, f)                                      \
         actionu_failchk(actionu_intparse_f((s), &(t), 1, (f)))

#define actionu_bufferparse(s, t)                                            \
{                                                                            \
  t = actionu_bufferparse_f((s));                                            \
                                                                             \
  if (!t)                                                                    \
  {                                                                          \
    printf(actionu_bufferparse_fail, (s), cfsh_num_buffers);                 \
    actionu_faili()                                                          \
  }                                                                          \
}

#define actionu_bufferchk(i, o)  actionu_failchk(actionu_bufferchk_f((i), (o)))
#define actionu_bufferfchk(b, f) actionu_failchk(actionu_bufferfchk_f((b), (f)))
#define actionu_bufferschk(i, o) actionu_failchk(actionu_bufferschk_f((i), (o)))

#define actionu_dictcheck()                                                  \
  if (dict == NULL)                                                          \
  {                                                                          \
    printf("dictionary not loaded. use `loaddict`\n");                       \
    actionu_faili()                                                          \
  }

