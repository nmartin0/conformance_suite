# csplit: the last line of each piece loses its newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref piece_nonl "$@"; }
