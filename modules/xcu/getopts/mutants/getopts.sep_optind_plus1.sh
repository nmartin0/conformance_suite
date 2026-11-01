# OPTIND after option and separate argument is one too large
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A reimplementation of getopts with one fault (see gorref.sh).
. "${PC_MUTANT_FILE%/*}/gorref.sh"
pcm_fault=sep_optind_plus1
pcm_install
