# -t c writes byte 127 as DEL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref c_del_raw "$@"; }
