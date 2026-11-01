# -t x8 transforms four bytes at a time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref sz8_to_4 "$@"; }
