# an existing output is overwritten without -f and the status is 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm overwrite_exit1 "$@"; }
