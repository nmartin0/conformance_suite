# xargs: -t separates the words of the traced line with a tab (spacing of the written line is not fixed)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref trace_tab "$@"; }
