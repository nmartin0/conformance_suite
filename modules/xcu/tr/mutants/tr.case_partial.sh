# case conversion covers only the first half of the alphabet
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref case_partial "$@"; }
