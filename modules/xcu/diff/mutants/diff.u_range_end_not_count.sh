# -u: the second number of a range is an ending line, not a count
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref u_range_end_not_count "$@"; }
