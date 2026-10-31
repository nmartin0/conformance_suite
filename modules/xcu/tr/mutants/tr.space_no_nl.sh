# [:space:] lacks the newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref space_no_nl "$@"; }
