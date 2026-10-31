# sort -n orders the fractions of negative numbers as if positive
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref n_neg_frac_wrong "$@"; }
