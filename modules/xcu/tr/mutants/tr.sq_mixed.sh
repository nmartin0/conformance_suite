# tr -s squeezes a run of different characters of the last operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref sq_mixed "$@"; }
