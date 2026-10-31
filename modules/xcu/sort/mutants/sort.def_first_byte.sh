# sort with no options compares only the first byte of each line, stably
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref def_first_byte "$@"; }
