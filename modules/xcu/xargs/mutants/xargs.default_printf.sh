# xargs: without a utility operand xargs runs a utility that writes one argument per line instead of echo
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref default_printf "$@"; }
