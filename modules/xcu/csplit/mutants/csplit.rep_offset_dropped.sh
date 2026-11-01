# csplit: a repeated expression loses its offset
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref rep_offset_dropped "$@"; }
