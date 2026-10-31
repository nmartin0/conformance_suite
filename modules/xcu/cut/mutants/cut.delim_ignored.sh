# -d is ignored: the tab remains the delimiter
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref delim_ignored f "$@"; }
