# uniq -d counts a line as repeated when it recurs anywhere
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref nonadj_d "$@"; }
