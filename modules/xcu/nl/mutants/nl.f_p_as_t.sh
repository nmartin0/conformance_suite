# -f pstring numbers the non-empty footer lines whatever the string
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PC_NLREF_DIR=${PC_MUTANT_FILE%/*}
. "$PC_NLREF_DIR/nlref.sh"
nl() { pc_nlref f_p_as_t "$@"; }
