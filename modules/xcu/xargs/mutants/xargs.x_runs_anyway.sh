# xargs: with -x a command line that does not fit is run anyway
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref x_runs_anyway "$@"; }
