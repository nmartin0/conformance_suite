# leading blanks of a line are not ignored (they start an empty first field)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref lead_blank_field "$@"; }
