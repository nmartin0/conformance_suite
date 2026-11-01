# -N larger than the input is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref N_short_err "$@"; }
