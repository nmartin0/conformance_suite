# the complement of string1 never contains the newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref c_excl_newline "$@"; }
