# -t a writes the space as an underscore
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref a_sp_underscore "$@"; }
