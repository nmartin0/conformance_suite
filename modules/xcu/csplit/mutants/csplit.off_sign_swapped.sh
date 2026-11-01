# csplit: the sign of the offset is reversed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref off_sign_swapped "$@"; }
