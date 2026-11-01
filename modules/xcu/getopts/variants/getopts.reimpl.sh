# conforming: plain reimplementation, OPTIND encoded inside a word
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming reimplementation of getopts (see mutants/gorref.sh).
. "${PC_MUTANT_FILE%/*}/../mutants/gorref.sh"
pcm_fault=reimpl
pcm_install
