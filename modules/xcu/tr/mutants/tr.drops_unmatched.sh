# characters that are not in string1 are dropped when translating
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref drops_unmatched "$@"; }
