# the offset operand is dropped but the dump starts at zero
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref xo_ignored "$@"; }
