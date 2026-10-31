# sort -n stops the number at the radix character
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref n_no_frac "$@"; }
