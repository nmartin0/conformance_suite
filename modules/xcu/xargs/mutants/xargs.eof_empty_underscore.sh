# xargs: with -E "" an underscore still ends the input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref eof_empty_underscore "$@"; }
