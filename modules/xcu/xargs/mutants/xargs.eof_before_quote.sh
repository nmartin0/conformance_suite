# xargs: the end-of-file string is compared before quote processing, so a quoted one does not end the input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref eof_before_quote "$@"; }
