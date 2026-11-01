# csplit: a line number operand ends the piece one line early
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref ln_minus1 "$@"; }
