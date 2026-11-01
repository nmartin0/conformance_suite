# xargs: xargs puts the initial arguments after the arguments read from standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref init_after "$@"; }
