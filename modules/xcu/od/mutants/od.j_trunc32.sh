# -j is taken modulo 2 to the 32
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref j_trunc32 "$@"; }
