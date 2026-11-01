# xargs: an apostrophe inside double quotes is dropped
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref dq_ends_at_sq "$@"; }
