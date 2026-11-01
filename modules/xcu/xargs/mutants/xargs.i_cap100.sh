# xargs: -I cuts a line longer than 100 bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/xargsref.sh"
xargs() { pc_xargsref i_cap100 "$@"; }
