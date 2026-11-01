# xargs: the exit status is that of the first failing invocation (allowed: no algorithm is fixed)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref status_first "$@"; }
