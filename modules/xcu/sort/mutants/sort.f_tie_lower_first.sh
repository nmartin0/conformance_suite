# sort -f orders lines that fold equal with lowercase first
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref f_tie_lower_first "$@"; }
