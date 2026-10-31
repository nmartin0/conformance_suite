# in [x*n] an escape for x stands for the letter after the backslash
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref rep_x_escape_raw "$@"; }
