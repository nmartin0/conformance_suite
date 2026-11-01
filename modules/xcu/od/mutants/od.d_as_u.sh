# signed decimal types write unsigned numbers
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref d_as_u "$@"; }
