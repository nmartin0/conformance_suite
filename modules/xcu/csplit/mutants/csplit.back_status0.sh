# csplit: an error (back_status0) gives status 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref back_status0 "$@"; }
