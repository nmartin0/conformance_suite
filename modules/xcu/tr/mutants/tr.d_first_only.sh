# tr -d deletes only the first occurrence of each character
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref d_first_only "$@"; }
