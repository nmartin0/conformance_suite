# xargs: xargs exits with status 1 after a successful run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref status1_ok "$@"; }
