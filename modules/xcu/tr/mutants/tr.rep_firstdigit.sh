# [x*n] reads only the first digit of n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref rep_firstdigit "$@"; }
