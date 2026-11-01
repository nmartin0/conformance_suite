# csplit: after an error (repeat_keeps) the files are kept
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref repeat_keeps "$@"; }
