# with -f the output file is not left in place
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm force_no_output "$@"; }
