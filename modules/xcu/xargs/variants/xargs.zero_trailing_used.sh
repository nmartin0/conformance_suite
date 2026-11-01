# xargs: with -0, bytes after the last null are used as the last argument (allowed: "may use them")
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref zero_trailing_used "$@"; }
