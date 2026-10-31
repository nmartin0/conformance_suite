# sort -f folds to lowercase instead of uppercase
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref f_lower "$@"; }
