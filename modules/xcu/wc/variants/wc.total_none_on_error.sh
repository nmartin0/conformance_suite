# a conforming wc: no total line is written when an operand could not be read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/wcref.sh"
wc() { pc_wcref total_none_on_error "$@"; }
