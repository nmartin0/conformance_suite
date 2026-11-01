# uudecode exits 0 for a missing input file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref missing_status0 "$@"; }
pc_uu_install missing_status0
