# a total line is also written for one operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref total_single "$@"; }
