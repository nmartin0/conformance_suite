# csplit: a repeat operand after a skip operand is ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref rep_skip_ignored "$@"; }
