# lines are compared with the first line, not the previous one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref star_cmp_first "$@"; }
