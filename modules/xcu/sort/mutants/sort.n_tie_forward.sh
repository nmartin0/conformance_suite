# sort -n -r orders lines of equal value ascending
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref n_tie_forward "$@"; }
