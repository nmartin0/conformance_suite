# uniq -f takes a field to be non-blanks followed by blanks
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref f_trailing "$@"; }
