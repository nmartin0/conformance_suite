# a missing argument with a leading colon puts the letter in the name variable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A reimplementation of getopts with one fault (see gorref.sh).
. "${PC_MUTANT_FILE%/*}/gorref.sh"
pcm_fault=mcolon_name_opt
pcm_install
