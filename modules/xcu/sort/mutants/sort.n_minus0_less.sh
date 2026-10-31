# sort -n orders a negative zero before zero
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref n_minus0_less "$@"; }
