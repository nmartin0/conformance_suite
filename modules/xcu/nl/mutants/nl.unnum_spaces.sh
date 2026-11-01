# an unnumbered line gets blanks the width of the number but no separator
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PC_NLREF_DIR=${PC_MUTANT_FILE%/*}
. "$PC_NLREF_DIR/nlref.sh"
nl() { pc_nlref unnum_spaces "$@"; }
