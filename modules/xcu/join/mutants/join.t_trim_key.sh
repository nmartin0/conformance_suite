# with -t leading blanks of the join field are ignored in comparing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref t_trim_key "$@"; }
