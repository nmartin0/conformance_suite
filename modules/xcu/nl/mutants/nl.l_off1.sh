# every (num+1)th blank line is numbered, not every numth
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PC_NLREF_DIR=${PC_MUTANT_FILE%/*}
. "$PC_NLREF_DIR/nlref.sh"
nl() { pc_nlref l_off1 "$@"; }
