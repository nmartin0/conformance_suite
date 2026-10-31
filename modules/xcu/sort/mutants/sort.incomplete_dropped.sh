# sort drops an incomplete last line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref incomplete_dropped "$@"; }
