# csplit: only the first digit of a repeat count is read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref rep_first_digit "$@"; }
