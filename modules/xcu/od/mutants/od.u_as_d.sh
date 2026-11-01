# unsigned decimal types write signed numbers
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref u_as_d "$@"; }
