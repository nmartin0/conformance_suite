# tr -cs squeezes by the characters of string1 instead of the replacement
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref cs_sq_s1 "$@"; }
