# csplit: only the first digit of a line number is read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref ln_first_digit "$@"; }
