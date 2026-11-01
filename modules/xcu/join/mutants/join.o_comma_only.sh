# an -o list separated by blanks is not accepted
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref o_comma_only "$@"; }
