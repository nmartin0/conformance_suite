# tr -s squeezes runs of the input characters rather than of the translated ones
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref sq_before_translate "$@"; }
