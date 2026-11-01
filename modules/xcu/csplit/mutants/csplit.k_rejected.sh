# csplit: -k is an invalid option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref k_rejected "$@"; }
