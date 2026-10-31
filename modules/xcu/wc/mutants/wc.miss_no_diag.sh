# an unreadable operand gets no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref miss_no_diag "$@"; }
