# csplit: a repeat that runs out of matches is not an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref rep_nomatch_ok "$@"; }
