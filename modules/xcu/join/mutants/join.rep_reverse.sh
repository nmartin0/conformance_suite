# the combinations are written in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref rep_reverse "$@"; }
