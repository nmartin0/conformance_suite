# sort leaves off the newline after the last line it writes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref no_final_nl "$@"; }
