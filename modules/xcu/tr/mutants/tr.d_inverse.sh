# tr -d deletes the characters that are not in string1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref d_inverse "$@"; }
