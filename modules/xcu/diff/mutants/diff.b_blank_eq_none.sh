# -b: all white space is ignored, so a blank equals none
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref b_blank_eq_none "$@"; }
