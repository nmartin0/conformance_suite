# xargs: no diagnostic message when the utility cannot be found or invoked
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref nf_no_diag "$@"; }
