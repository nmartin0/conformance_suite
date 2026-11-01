# csplit: the line named by a line number operand is in no file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref boundary_lost "$@"; }
