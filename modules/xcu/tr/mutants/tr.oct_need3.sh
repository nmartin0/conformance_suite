# an octal sequence of fewer than three digits is not recognised
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref oct_need3 "$@"; }
