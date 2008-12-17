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

#ifndef STDINC_CIFER
#define STDINC_CIFER 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "settings.h"
#include "macros.h"
#include "command_info.h"

#include "frequency_analysis.h"
#include "frequency_data.h"

#include "affine.h"
#include "bacon.h"
#include "ciphers.h"
#include "columnar_transposition.h"
#include "keyword.h"
#include "polybius.h"
#include "rf.h"
#include "vigenere.h"

#include "command.h"
#include "io.h"
#include "shell.h"
#include "actions.h"

#include "dictionary.h"
#include "interface.h"
#include "utility.h"

#endif

