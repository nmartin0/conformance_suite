# csplit: lines longer than 1000 bytes are cut
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref long_line_cut "$@"; }
