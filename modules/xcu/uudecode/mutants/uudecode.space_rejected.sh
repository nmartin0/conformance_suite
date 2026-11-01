# uudecode rejects a space in a data line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref space_rejected "$@"; }
pc_uu_install space_rejected
