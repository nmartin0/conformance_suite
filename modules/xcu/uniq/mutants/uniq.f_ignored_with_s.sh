# uniq ignores -f when -s is given
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref f_ignored_with_s "$@"; }
