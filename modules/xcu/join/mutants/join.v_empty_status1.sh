# the status is 1 when -v writes nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref v_empty_status1 "$@"; }
