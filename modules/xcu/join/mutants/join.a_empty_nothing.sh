# nothing is written when one of the files is empty
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref a_empty_nothing "$@"; }
