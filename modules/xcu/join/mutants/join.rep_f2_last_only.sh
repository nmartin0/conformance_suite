# only the last matching line of file2 is used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref rep_f2_last_only "$@"; }
