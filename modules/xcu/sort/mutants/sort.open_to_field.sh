# a -k key without an end stops at the end of its first field
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref open_to_field "$@"; }
