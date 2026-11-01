# csplit: the search starts at the current line, not after it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref match_from_cur "$@"; }
