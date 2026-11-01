# an -o list is separated by spaces, not by tabs
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref o_space_only "$@"; }
