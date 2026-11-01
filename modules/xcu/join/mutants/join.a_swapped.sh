# -a 1 adds the lines of file2 and -a 2 those of file1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref a_swapped "$@"; }
