# setting OPTIND to 1 keeps the unused part of a word
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A reimplementation of getopts with one fault (see gorref.sh).
. "${PC_MUTANT_FILE%/*}/gorref.sh"
pcm_fault=reset_keeps_cluster
pcm_install
