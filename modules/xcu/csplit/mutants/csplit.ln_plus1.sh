# csplit: a line number operand ends the piece one line late
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref ln_plus1 "$@"; }
