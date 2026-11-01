# the output lacks its final newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref no_final_nl "$@"; }
