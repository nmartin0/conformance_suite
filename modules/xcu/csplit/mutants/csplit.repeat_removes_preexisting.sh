# csplit: after an error (repeat_removes_preexisting) every file with the prefix is removed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref repeat_removes_preexisting "$@"; }
