# -t c writes other controls with two octal digits
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref c_ctl_short "$@"; }
