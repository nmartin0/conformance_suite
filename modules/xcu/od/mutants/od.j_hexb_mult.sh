# -j 0x..b takes the b as a multiplier
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref j_hexb_mult "$@"; }
