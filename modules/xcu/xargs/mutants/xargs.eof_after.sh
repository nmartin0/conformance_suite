# xargs: the end-of-file string is dropped but the input after it is still used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref eof_after "$@"; }
