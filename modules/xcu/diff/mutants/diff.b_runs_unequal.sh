# -b: trailing white space is ignored but runs of blanks of different length differ
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref b_runs_unequal "$@"; }
