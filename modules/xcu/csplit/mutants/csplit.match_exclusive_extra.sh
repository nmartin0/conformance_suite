# csplit: the piece ends one line before the line before the match
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref match_exclusive_extra "$@"; }
