# -t x2 transforms one byte at a time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref sz2_to_1 "$@"; }
