# [:space:] lacks the vertical tab
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref space_no_vt "$@"; }
