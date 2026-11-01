# uudecode requires a file operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref stdin_none "$@"; }
pc_uu_install stdin_none
