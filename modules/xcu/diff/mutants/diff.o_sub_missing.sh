# -r: files present only in a subdirectory are not reported
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref o_sub_missing "$@"; }
