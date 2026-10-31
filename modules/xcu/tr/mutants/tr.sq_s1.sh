# tr -s squeezes by the characters of string1 rather than the last operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref sq_s1 "$@"; }
