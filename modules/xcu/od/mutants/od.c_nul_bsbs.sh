# -t c writes NUL as two backslashes and 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref c_nul_bsbs "$@"; }
