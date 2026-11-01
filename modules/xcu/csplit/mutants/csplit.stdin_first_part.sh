# csplit: only the first 5 lines of standard input are used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref stdin_first_part "$@"; }
