# a missing argument with a leading colon sets OPTARG to a colon
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A reimplementation of getopts with one fault (see gorref.sh).
. "${PC_MUTANT_FILE%/*}/gorref.sh"
pcm_fault=mcolon_optarg_colon
pcm_install
