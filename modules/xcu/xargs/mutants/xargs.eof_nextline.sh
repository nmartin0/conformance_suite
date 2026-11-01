# xargs: the end-of-file string only discards the rest of its line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref eof_nextline "$@"; }
