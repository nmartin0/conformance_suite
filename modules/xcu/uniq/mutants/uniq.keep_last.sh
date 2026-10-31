# uniq writes the last copy of a run instead of the first
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref keep_last "$@"; }
