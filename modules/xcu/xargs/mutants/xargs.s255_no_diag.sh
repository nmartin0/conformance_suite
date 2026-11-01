# xargs: status 255 stops xargs without a diagnostic message
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref s255_no_diag "$@"; }
