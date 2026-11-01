# csplit: a later line number is counted from the current line, not from the start
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref ln_relative "$@"; }
