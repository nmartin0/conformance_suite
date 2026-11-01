# csplit: the diagnostic for an error (past_diag_stdout) goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref past_diag_stdout "$@"; }
