# -t a writes byte 127 as 177
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref a_del_octal "$@"; }
