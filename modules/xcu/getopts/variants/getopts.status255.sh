# conforming: an error in setting a variable exits 255
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming reimplementation of getopts (see mutants/gorref.sh).
. "${PC_MUTANT_FILE%/*}/../mutants/gorref.sh"
pcm_fault=status255
pcm_install
