# -b: trailing white space is collapsed but not ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref b_trail_kept "$@"; }
