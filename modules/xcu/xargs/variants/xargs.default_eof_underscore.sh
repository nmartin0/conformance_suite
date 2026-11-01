# xargs: the logical end-of-file string is an underscore unless -E says otherwise (allowed: the default is unspecified)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref default_eof_underscore "$@"; }
