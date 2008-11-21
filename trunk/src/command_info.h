
#define dict_not_loaded "dictionary not loaded: type `usage load_dict' for more info\n"

#define action_default_usage "usage: help (to list all commands) or usage <command name>";
#define action_default_use "no such command";

#define action_buffers_usage "usage: buffers #  (num of buffers, > 0)\n"
#define action_quit_usage "usage: quit <no args>\n"
#define action_resize_usage "usage: buffer_# X (tgt buffer, new_size)\n"
#define action_clear_usage "usage: clear buffer_#\n"
#define action_copy_usage "usage: copy buffer_# buffer_# (src, tgt)\n"
#define action_load_usage "usage: load filename buffer_# (src, tgt)\n"
#define action_write_usage "usage: write filename buffer_# append|overwrite|auto (tgt, src)\n"
#define action_filter_usage "usage: buffer_# mode where mode is one of:  \n\
  alpha     - Only alpha (A-Za-z) chars allowed. \n\
  alphanum  - Only alphanumeric (A-Za-z0-9) chars allowed. \n\
  lalpha    - Only alpha chars allowed, all changed to lowercase. \n\
  ualpha    - Only alpha chars allowed, all changed to uppercase. \n\
  flipcase  - Only alpha chars allowed, upper/lowercase flipped. \n\
  casebacon - Only alpha, all changed to AB from upper/lower. \n\
  bacon     - Only A or Bs \n\
  num       - Only numbers allowed. \n\
  esp       - Removes all whitespace. \n\
  enl       - Removes all whitespace except standard ' ' spaces. \n"
#define action_read_usage "usage: read buffer_#\n"
#define action_bufferinfo_usage "usage: bufferinfo buffer_#\n"
#define action_nullbuffer_usage "usage: nullbuffer buffer_#\n"
#define action_script_usage "usage: [quick][soft]script filename\n"
#define action_quickscript_usage "usage: [quick][soft]script filename\n"
#define action_softscript_usage "usage: [quick][soft]script filename\n"
#define action_quicksoftscript_usage "usage: [quick][soft]script filename\n"
#define action_score_usage "usage: score buffer_#\n"
#define action_spaces_usage "usage: spaces buffer_# buffer_# (input, output)\n"
#define action_wordwrap_usage "usage: wordwrap buffer_# buffer_# (input, output)\n"
#define action_loaddict_usage "usage: loaddict <no args>\n\
 a dictionary can be created using the cifer-dict command \n\
 (external to this shell), see the cifer-dict manpage for more info.\n\
 Some functions will be unavailable. The location of the dictionary can be \n\
 changed from its default by using the cifer command `dict_location` \n\
 (a function of the shell) \n"
#define action_dictlocation_usage "usage: dictlocation <new location: filename>\n"
#define action_affine_usage "usage: affine buffer_# buffer_# (input, output)\n"
#define action_affinesolve_usage "usage: affinesolve ct1 pt1 ct2 pt2\n"
#define action_affinebf_usage "usage: affinebf buffer_# buffer_# (input, output)\n"
#define action_affineencode_usage "usage: affineencode buffer_# buffer_# # # (input, output, a, b)\n"
#define action_affinedecode_usage "usage: affinedecode buffer_# buffer_# # # (input, output, a, b)\n"
#define action_baconencode_usage "usage: baconencode buffer_# buffer_# (input, output)\n"
#define action_bacondecode_usage "usage: bacondecode buffer_# buffer_# (input, output)\n"
#define action_shift_usage "usage: shift buffer_# buffer_# forwards|backwards shift \n\
  (input, output, encode|decode - direction, shift(s) in alpha or num)\n"
#define action_deltaic_usage "usage: deltai buffer_#\n"
#define action_monoalph_usage "usage: monoalph buffer_# buffer_# encrypt|decrypt ctext_targets \n\
  (input, output, cipher mode, ctext_targets for abcd....xyz (alpha size 26))\n"
#define action_ctrans_keyinfo_usage "usage: keyword (in alpha (eg. tomato) or numbers (1 3 2 0) \n"
#define action_c2c_encode_usage "usage: c2c_encode buffer_# buffer_# keyword\n\
  (input, output, keyword in alpha (eg. tomato) or numbers (1 3 2 0) \n"
#define action_r2c_encode_usage "usage: r2c_encode buffer_# buffer_# keyword\n\
  (input, output, keyword in alpha (eg. tomato) or numbers (1 3 2 0) \n"
#define action_c2r_encode_usage "usage: c2r_encode buffer_# buffer_# keyword\n\
  (input, output, keyword in alpha (eg. tomato) or numbers (1 3 2 0) \n"
#define action_c2c_decode_usage "usage: c2c_decode buffer_# buffer_# keyword\n\
  (input, output, keyword in alpha (eg. tomato) or numbers (1 3 2 0) \n"
#define action_r2c_decode_usage "usage: r2c_decode buffer_# buffer_# keyword\n\
  (input, output, keyword in alpha (eg. tomato) or numbers (1 3 2 0) \n"
#define action_c2r_decode_usage "usage: c2r_decode buffer_# buffer_# keyword\n\
  (input, output, keyword in alpha (eg. tomato) or numbers (1 3 2 0) \n"
#define action_c2c_bruteforce_usage "usage: c2c_bruteforce buffer_# buffer_# \n\
  (input, output) \n"
#define action_r2c_bruteforce_usage "usage: r2c_bruteforce buffer_# buffer_# \n\
  (input, output) \n"
#define action_c2r_bruteforce_usage "usage: c2r_bruteforce buffer_# buffer_# \n\
  (input, output) \n"
#define action_fg_usage "usage: fg buffer_# [buffer_#]\n"
#define action_ifg_usage "usage: ifg text_size|buffer_#\n"
#define action_fa_usage "usage: fa buffer_#\n"
#define action_digrams_usage "usage: digrams buffer_# num_to_show\n"
#define action_trigrams_usage "usage: trigrams buffer_# num_to_show\n"
#define action_pct_usage "usage: pct <no args>\n"
#define action_keyb_usage "usage: keyb buffer_# buffer_# (input, output)\n"
#define action_keyt_usage "usage: keyt keyword_text\n"
#define action_keytf_usage "usage: keytf keyword_text\n"
#define action_keyd_usage "usage: keyd buffer_# buffer_# keyword_text [flipped] (input, output)\n"
#define action_polybius_usage "usage: polybius buffer_# buffer_#\n"
#define action_rfbf_usage "usage: rfbf buffer_# buffer_# min max (input, output, bf_range)\n"
#define action_vigenere_usage "usage: vigenere buffer_# buffer_# min max (input, output, bf_range)\n"
#define action_mmi_usage "usage: mmi int_value int_mod\n"
#define action_gcd_usage "usage: gcd int_arg1 int_arg2 ...\n"
#define action_coprime_usage "usage: coprime int_arg1 int_arg2\n"
#define action_charinfo_usage "usage: charinfo char|num\n"
#define action_usage_usage "usage: usage command_name\n"
#define action_help_usage "usage: help <no args> (use usage <command name> for spesific-commands)\n"
#define action_system_usage "usage: system command arg1 arg2 ...\n"
#define action_cd_usage "usage: cd new_directory (can be relative or definite)\n"
#define action_ls_usage "usage: ls directory_to_list (can be omitted to list current directory)\n"

#define action_buffers_use "buffers will modify the buffer_array by allocating or destroying buffers.\n"
#define action_quit_use "quit will exit the shell\n"
#define action_resize_use "resize modifies the size-in-bytes of any given buffer, increase or decrease\n"
#define action_clear_use "clear empties the buffer and sets every byte of it to \\0 \n"
#define action_copy_use "copy will copy one buffer's contents into another, resizing if needed\n"
#define action_load_use "load opens a file and copies its contents into a buffer, unmodified\n"
#define action_write_use "write puts the contents of a buffer into a file\n"
#define action_filter_use "filter will filters a buffer, to prepare it for a crack\n"
#define action_read_use "read displays both the contents and the status of a buffer\n"
#define action_bufferinfo_use "bufferinfo will display the status and size of a buffer\n"
#define action_nullbuffer_use "nullbuffer is a safety function that sets the final null byte\n"
#define action_script_use "script executes any given file as if it were typed into the shell\n"
#define action_quickscript_use "quickscript is the same as script except it does not preparse\n"
#define action_softscript_use "softscript is the same as script except it will stop on soft errors aswell\n"
#define action_quicksoftscript_use "quicksoftscript is a combination of quickscript and softscript\n"
#define action_score_use "score will return a 'english-score' on a buffer from the dictionary\n"
#define action_spaces_use "spaces will attempt to add spaces to a ctext based on dictionary lookups\n"
#define action_wordwrap_use "wordwrap will use spaces and try to wrap the text to the specified length\n"
#define action_loaddict_use "loaddict loads the dictionary into memory if it is not already so\n"
#define action_dictlocation_use "dictlocation sets the location of the dictionary for the loaddict command\n"
#define action_affine_use "affine performs an affine crack by trying to find THE and using affinesolve\n"
#define action_affinesolve_use "affinesolve uses modular arithmitic to solve an affine simultaneous equation\n"
#define action_affinebf_use "affinebf works alike affine except it uses bruteforce to find the solution\n"
#define action_affineencode_use "affineencode will encrypt a buffer with the affine cipher\n"
#define action_affinedecode_use "affinedecode will decrypt a buffer with the affine cipher\n"
#define action_baconencode_use "baconencode encrypts alpha chars using the bacon cipher\n"
#define action_bacondecode_use "bacondecode takes a stream of As and Bs and decrypts using the bacon cipher\n"
#define action_shift_use "shift takes a buffer and performs a vigenere style shift on it\n"
#define action_deltaic_use "deltaic computes the index of coincidence on a buffer\n"
#define action_monoalph_use "monoalph translates a buffer using monoalphabetic substitution\n"
#define action_ctrans_keyinfo_use "ctrans_keyinfo gives information on a columnar transposition key\n"
#define action_c2c_encode_use "c2c_encode encrypts using column to column transposition\n"
#define action_c2c_decode_use "c2c_decode decrypts using column to coulmn transposition\n"
#define action_c2c_bruteforce_use "c2c_bruteforce bruteforces the key for column to column transposition\n"
#define action_r2c_encode_use "r2c_encode encrypts using row to column transposition\n"
#define action_r2c_decode_use "r2c_decode decrypts using row to column transposition\n"
#define action_r2c_bruteforce_use "r2c_bruteforce bruteforces the key for row to column transposition\n"
#define action_c2r_encode_use "c2r_encode encrypts using column to row transposition\n"
#define action_c2r_decode_use "c2r_decode decrypts using column to row transposition\n"
#define action_c2r_bruteforce_use "c2r_bruteforce bruteforces the key for column to row transposition\n"
#define action_fg_use "fg uses frequency analysis to try and guess the characters\n"
#define action_ifg_use "ifg makes an 'identity' freq. graph for any text_size (expected frequencies)\n"
#define action_fa_use "fa performs frequency analysis on a buffer\n"
#define action_digrams_use "digrams reads the ctext and finds the top N digrams\n"
#define action_trigrams_use "trigrams reads the ctext and finds the top N trigrams\n"
#define action_pct_use "pct prints the character table (0 = A, 1 = B...)\n"
#define action_keyb_use "keyb preforms keyword bruteforce\n"
#define action_keyt_use "keyt creates a keyword-table\n"
#define action_keytf_use "keytf creates a flipped keyword-table\n"
#define action_keyd_use "keyd decrypts a keyword-cipher using the keyword or table supplied\n"
#define action_polybius_use "polybius will decrypt a text using the polybius cipher\n"
#define action_rfbf_use "rfbf bruteforces a railfence cipher\n"
#define action_vigenere_use "vigenere uses deltaic and frequency analysis to crack a vigenere cipher\n"
#define action_mmi_use "mmi computes the modular multiplicative inverse, used in affine ciphers\n"
#define action_gcd_use "gcd computes the greatest common denominator of any number of integers\n"
#define action_coprime_use "coprime determines if two integers are coprime\n"
#define action_charinfo_use "charinfo print information about a character\n"
#define action_usage_use "usage prints the syntax of a function\n"
#define action_help_use "help lists all the functions\n"
#define action_system_use "system passes a command to /bin/sh\n"
#define action_cd_use "cd changes the current directory\n"
#define action_ls_use "ls lists the content of the current directory\n"

