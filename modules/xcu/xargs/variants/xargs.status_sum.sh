# xargs: the exit status is the sum of the statuses of the failing invocations
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref status_sum "$@"; }
