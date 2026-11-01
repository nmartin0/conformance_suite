# uudecode stops the output at the first NUL byte
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_nul_stop "$@"; }
pc_uu_install r_nul_stop
