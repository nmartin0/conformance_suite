# csplit: after a skip the next piece starts one line early
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref skip_cur_minus1 "$@"; }
