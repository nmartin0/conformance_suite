# xargs: xargs exits 0 after failing invocations
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref fail_status0 "$@"; }
