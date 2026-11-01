# a trailing b does not multiply by 512
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref xo_b_ignored "$@"; }
