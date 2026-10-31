# uniq ignores trailing blanks when comparing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref trailing_blank_ignored "$@"; }
