# csplit: a skip operand writes a file for the skipped lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref skip_writes_file "$@"; }
