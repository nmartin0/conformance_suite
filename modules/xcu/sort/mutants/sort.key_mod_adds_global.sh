# type letters of a -k key add to the global options instead of overriding them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref key_mod_adds_global "$@"; }
