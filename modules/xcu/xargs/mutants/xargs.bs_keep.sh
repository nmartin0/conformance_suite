# xargs: a backslash is kept in the argument together with the character it escapes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref bs_keep "$@"; }
