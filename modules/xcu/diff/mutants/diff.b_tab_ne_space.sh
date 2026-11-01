# -b: a tab and blanks compare unequal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref b_tab_ne_space "$@"; }
