# uudecode skips at most one line before the begin line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref lead_one "$@"; }
pc_uu_install lead_one
