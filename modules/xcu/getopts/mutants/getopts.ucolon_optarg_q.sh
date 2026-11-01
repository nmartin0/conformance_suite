# an unknown option with a leading colon sets OPTARG to a question mark
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A reimplementation of getopts with one fault (see gorref.sh).
. "${PC_MUTANT_FILE%/*}/gorref.sh"
pcm_fault=ucolon_optarg_q
pcm_install
