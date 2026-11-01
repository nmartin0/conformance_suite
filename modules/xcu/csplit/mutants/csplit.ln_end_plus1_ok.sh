# csplit: the line number just past the end is accepted
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref ln_end_plus1_ok "$@"; }
