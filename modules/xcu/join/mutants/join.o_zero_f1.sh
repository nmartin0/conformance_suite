# 0 in an -o list is the first field of file1, not the join field
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref o_zero_f1 "$@"; }
