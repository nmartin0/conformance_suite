# -t a swaps the names of vertical tab and form feed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref a_vt_ff "$@"; }
