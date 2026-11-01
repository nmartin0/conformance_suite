# a -j beyond the input gives no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref j_short_silent "$@"; }
