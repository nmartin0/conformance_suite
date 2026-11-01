# with one -d character the second delimiter character is a backslash
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PC_NLREF_DIR=${PC_MUTANT_FILE%/*}
. "$PC_NLREF_DIR/nlref.sh"
nl() { pc_nlref d_single_bs "$@"; }
