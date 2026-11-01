# field numbers in an -o list are off by one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref o_off_by_one "$@"; }
