# -t x4 transforms two bytes at a time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref sz4_to_2 "$@"; }
