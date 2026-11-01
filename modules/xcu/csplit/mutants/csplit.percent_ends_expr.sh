# csplit: an escaped percent sign ends the expression
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref percent_ends_expr "$@"; }
