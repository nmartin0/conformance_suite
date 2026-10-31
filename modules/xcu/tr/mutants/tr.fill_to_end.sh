# [x*] fills to the end of string1 and pushes out what follows it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref fill_to_end "$@"; }
