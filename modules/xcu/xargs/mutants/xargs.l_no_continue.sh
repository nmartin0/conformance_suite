# xargs: a trailing blank does not continue the line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref l_no_continue "$@"; }
