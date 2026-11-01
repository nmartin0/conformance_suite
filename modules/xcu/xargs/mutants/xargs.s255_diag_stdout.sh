# xargs: the diagnostic for status 255 goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref s255_diag_stdout "$@"; }
