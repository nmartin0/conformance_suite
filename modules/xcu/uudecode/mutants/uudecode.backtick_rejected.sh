# uudecode rejects a backtick in a data line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref backtick_rejected "$@"; }
pc_uu_install backtick_rejected
