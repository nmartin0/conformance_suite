# default format: the end of a file1 range is one too high
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref def_range1_end_off "$@"; }
