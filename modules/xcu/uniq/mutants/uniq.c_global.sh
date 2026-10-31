# uniq -c counts the occurrences in the whole input, not in the run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref c_global "$@"; }
