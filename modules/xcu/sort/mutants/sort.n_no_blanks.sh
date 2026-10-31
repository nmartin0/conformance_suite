# sort -n takes a number with leading blanks as zero
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref n_no_blanks "$@"; }
