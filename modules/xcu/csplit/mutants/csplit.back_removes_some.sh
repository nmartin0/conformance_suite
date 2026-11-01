# csplit: after an error (back_removes_some) only the last file is removed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref back_removes_some "$@"; }
