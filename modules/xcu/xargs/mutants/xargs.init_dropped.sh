# xargs: xargs drops the initial arguments of the utility operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref init_dropped "$@"; }
