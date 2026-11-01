# csplit: the diagnostic for a too long file name goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref long_diag_stdout "$@"; }
