# a missing argument with a leading colon sets the name to a question mark
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A reimplementation of getopts with one fault (see gorref.sh).
. "${PC_MUTANT_FILE%/*}/gorref.sh"
pcm_fault=mcolon_name_q
pcm_install
