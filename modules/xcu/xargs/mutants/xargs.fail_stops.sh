# xargs: xargs stops at the first invocation that fails (other than 255)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref fail_stops "$@"; }
