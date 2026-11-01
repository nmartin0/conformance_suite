# uudecode rejects a lone backtick as the last data line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref term_backtick_rejected "$@"; }
pc_uu_install term_backtick_rejected
