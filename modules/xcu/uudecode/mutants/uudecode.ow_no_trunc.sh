# uudecode overwrites an existing file in place without shortening it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref ow_no_trunc "$@"; }
pc_uu_install ow_no_trunc
