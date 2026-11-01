# conforming: diagnostics give the program name as invoked, not its base name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming reimplementation of getopts (see mutants/gorref.sh).
. "${PC_MUTANT_FILE%/*}/../mutants/gorref.sh"
pcm_fault=diag_fullpath
pcm_install
