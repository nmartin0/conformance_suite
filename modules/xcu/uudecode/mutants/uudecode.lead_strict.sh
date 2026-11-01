# uudecode requires the begin line to be the first line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref lead_strict "$@"; }
pc_uu_install lead_strict
