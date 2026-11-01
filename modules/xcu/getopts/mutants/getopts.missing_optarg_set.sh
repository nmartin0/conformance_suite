# a missing argument sets OPTARG to the letter without a leading colon
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A reimplementation of getopts with one fault (see gorref.sh).
. "${PC_MUTANT_FILE%/*}/gorref.sh"
pcm_fault=missing_optarg_set
pcm_install
