# the join field stays in its place in file1 instead of coming first
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref key_stays "$@"; }
