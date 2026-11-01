# csplit: -s sends the size lines to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref s_to_stderr "$@"; }
