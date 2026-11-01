# -t c writes byte 27 as backslash e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref c_esc_e "$@"; }
