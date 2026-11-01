# csplit: with -k an error has no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref k_error_no_diag "$@"; }
