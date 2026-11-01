# an unsupported algorithm gives status 2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm unsupp_status2 "$@"; }
