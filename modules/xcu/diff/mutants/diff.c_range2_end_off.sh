# -c: the end of the file2 range is one too high
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref c_range2_end_off "$@"; }
