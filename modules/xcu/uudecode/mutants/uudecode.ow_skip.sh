# uudecode leaves an existing file alone and exits 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref ow_skip "$@"; }
pc_uu_install ow_skip
