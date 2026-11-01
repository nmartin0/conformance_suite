# csplit: a skip operand ignores its offset
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref skip_ignores_offset "$@"; }
