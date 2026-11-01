# csplit: an escaped slash ends the expression
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref slash_ends_expr "$@"; }
