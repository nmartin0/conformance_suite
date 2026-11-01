# only the first matching line of file1 is used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref rep_f1_first_only "$@"; }
