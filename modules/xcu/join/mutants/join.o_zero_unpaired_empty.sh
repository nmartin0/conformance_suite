# 0 in an -o list is empty for an unpairable line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref o_zero_unpaired_empty "$@"; }
