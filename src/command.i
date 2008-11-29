  cfsh_func("buffers"                     , action_buffers)
  cfsh_func("quit"                        , action_quit)
  cfsh_func("exit"                        , action_quit)
  cfsh_func("resize"                      , action_resize)
  cfsh_func("clear"                       , action_clear)
  cfsh_func("copy"                        , action_copy)
  cfsh_func("load"                        , action_load)
  cfsh_func("write"                       , action_write)
  cfsh_func("filter"                      , action_filter)
  cfsh_func("read"                        , action_read)
  cfsh_func("print"                       , action_read)
  cfsh_func("bufferinfo"                  , action_bufferinfo)
  cfsh_func("getsize"                     , action_bufferinfo)
  cfsh_func("getfilter"                   , action_bufferinfo)
  cfsh_func("nullbuffer"                  , action_nullbuffer)
  cfsh_func("file"                        , action_script)
  cfsh_func("script"                      , action_script)
  cfsh_func("quickfile"                   , action_quickscript)
  cfsh_func("quickscript"                 , action_quickscript)
  cfsh_func("softfile"                    , action_softscript)
  cfsh_func("softscript"                  , action_softscript)
  cfsh_func("quicksoftfile"               , action_quicksoftscript)
  cfsh_func("quicksoftscript"             , action_quicksoftscript)
  cfsh_func("score"                       , action_score)
  cfsh_func("spaces"                      , action_spaces)
  cfsh_func("wordwrap"                    , action_wordwrap)
  cfsh_func("ww"                          , action_wordwrap)
  cfsh_func("wrap"                        , action_wordwrap)
  cfsh_func("loaddict"                    , action_loaddict)
  cfsh_func("load_dict"                   , action_loaddict)
  cfsh_func("affine"                      , action_affine)
  cfsh_func("crack_affine"                , action_affine)
  cfsh_func("affinesolve"                 , action_affinesolve)
  cfsh_func("affine_solve"                , action_affinesolve)
  cfsh_func("affinebf"                    , action_affinebf)
  cfsh_func("affine_bf"                   , action_affinebf)
  cfsh_func("affine_encode"               , action_affineencode)
  cfsh_func("affineencode"                , action_affineencode)
  cfsh_func("affinee"                     , action_affineencode)
  cfsh_func("affine_decode"               , action_affinedecode)
  cfsh_func("affinedecode"                , action_affinedecode)
  cfsh_func("affined"                     , action_affinedecode)
  cfsh_func("baconencode"                 , action_baconencode)
  cfsh_func("bacon_encode"                , action_baconencode)
  cfsh_func("baconenc"                    , action_baconencode)
  cfsh_func("bacondecode"                 , action_bacondecode)
  cfsh_func("bacon"                       , action_bacondecode)
  cfsh_func("bacon_decode"                , action_bacondecode)
  cfsh_func("bacondec"                    , action_bacondecode)
  cfsh_func("crack_bacon"                 , action_bacondecode)
  cfsh_func("shift"                       , action_shift)
  cfsh_func("vigenere_manual"             , action_shift)
  cfsh_func("deltaic"                     , action_deltaic)
  cfsh_func("delta_ic"                    , action_deltaic)
  cfsh_func("monoalph"                    , action_monoalph)
  cfsh_func("ctrans_keyinfo"              , action_ctrans_keyinfo)
  cfsh_func("keyinfo"                     , action_ctrans_keyinfo)
  cfsh_func("c2c_encode"                  , action_c2c_encode)
  cfsh_func("c2c_decode"                  , action_c2c_decode)
  cfsh_func("c2c_bruteforce"              , action_c2c_bruteforce)
  cfsh_func("r2c_encode"                  , action_r2c_encode)
  cfsh_func("r2c_decode"                  , action_r2c_decode)
  cfsh_func("r2c_bruteforce"              , action_r2c_bruteforce)
  cfsh_func("c2r_encode"                  , action_c2r_encode)
  cfsh_func("c2r_decode"                  , action_c2r_decode)
  cfsh_func("c2r_bruteforce"              , action_c2r_bruteforce)
  cfsh_func("fg"                          , action_fg)
  cfsh_func("frequency_guesses"           , action_fg)
  cfsh_func("ifg"                         , action_ifg)
  cfsh_func("identity_frequency_graph"    , action_ifg)
  cfsh_func("fa"                          , action_fa)
  cfsh_func("count_freq"                  , action_fa)
  cfsh_func("frequency"                   , action_fa)
  cfsh_func("frequency_analysis"          , action_fa)
  cfsh_func("digrams"                     , action_digrams)
  cfsh_func("count_digrams"               , action_digrams)
  cfsh_func("trigrams"                    , action_trigrams)
  cfsh_func("count_trigrams"              , action_trigrams)
  cfsh_func("pct"                         , action_pct)
  cfsh_func("chartable"                   , action_pct)
  cfsh_func("keyb"                        , action_keyb)
  cfsh_func("keyword_bruteforce"          , action_keyb)
  cfsh_func("keye"                        , action_keye)
  cfsh_func("keyword_encode"              , action_keye)
  cfsh_func("keyd"                        , action_keyd)
  cfsh_func("keyword_decode"              , action_keyd)
  cfsh_func("keyt"                        , action_keyt)
  cfsh_func("keyword_table"               , action_keyt)
  cfsh_func("keytf"                       , action_keytf)
  cfsh_func("keyword_table_flipped"       , action_keytf)
  cfsh_func("polybius_encode"             , action_polybius_encode)
  cfsh_func("polybiusencode"              , action_polybius_encode)
  cfsh_func("polybius_decode"             , action_polybius_decode)
  cfsh_func("polybiusdecode"              , action_polybius_decode)
  cfsh_func("polybius"                    , action_polybius_decode)
  cfsh_func("rfbf"                        , action_rfbf)
  cfsh_func("railfence"                   , action_rfbf)
  cfsh_func("rf"                          , action_rfbf)
  cfsh_func("vigenere"                    , action_vigenere)
  cfsh_func("vigenere_crack"              , action_vigenere)
  cfsh_func("mmi"                         , action_mmi)
  cfsh_func("modmulinv"                   , action_mmi)
  cfsh_func("gcd"                         , action_gcd)
  cfsh_func("greatest_common_denominator" , action_gcd)
  cfsh_func("coprime"                     , action_coprime)
  cfsh_func("is_coprime"                  , action_coprime)
  cfsh_func("charinfo"                    , action_charinfo)
  cfsh_func("char_info"                   , action_charinfo)
  cfsh_func("char"                        , action_charinfo)
  cfsh_func("usage"                       , action_usage)
  cfsh_func("info"                        , action_usage)
  cfsh_func("help"                        , action_help)
  cfsh_func("system"                      , action_system)
  cfsh_func("exec"                        , action_system)
  cfsh_func("-"                           , action_system)
  cfsh_func(";"                           , action_system)
  cfsh_func("s"                           , action_system)
  cfsh_func("cd"                          , action_cd)
  cfsh_func("cwd"                         , action_cd)
  cfsh_func("ls"                          , action_ls)
  cfsh_func("list"                        , action_ls)
  cfsh_func("dir"                         , action_ls)
