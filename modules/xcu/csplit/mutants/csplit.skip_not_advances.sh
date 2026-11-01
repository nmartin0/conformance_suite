# csplit: a skip operand does not move the current line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref skip_not_advances "$@"; }
