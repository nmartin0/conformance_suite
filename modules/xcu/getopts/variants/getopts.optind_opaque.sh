# conforming: OPTIND after any exit-0 call but an option with argument is an opaque number
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming reimplementation of getopts (see mutants/gorref.sh).
. "${PC_MUTANT_FILE%/*}/../mutants/gorref.sh"
pcm_fault=optind_opaque
pcm_install
