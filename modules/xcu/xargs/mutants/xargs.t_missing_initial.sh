# xargs: -t leaves out the initial arguments of the utility operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref t_missing_initial "$@"; }
