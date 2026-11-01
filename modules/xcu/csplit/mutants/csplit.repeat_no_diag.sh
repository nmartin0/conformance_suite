# csplit: an error (repeat_no_diag) gives no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref repeat_no_diag "$@"; }
