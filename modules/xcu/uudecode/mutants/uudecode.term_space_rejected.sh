# uudecode rejects a lone space as the last data line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref term_space_rejected "$@"; }
pc_uu_install term_space_rejected
