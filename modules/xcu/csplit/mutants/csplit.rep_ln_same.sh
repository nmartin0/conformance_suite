# csplit: a repeated line number reuses the same line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref rep_ln_same "$@"; }
