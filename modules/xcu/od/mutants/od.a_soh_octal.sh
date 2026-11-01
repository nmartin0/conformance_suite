# -t a writes byte 1 as 001
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref a_soh_octal "$@"; }
