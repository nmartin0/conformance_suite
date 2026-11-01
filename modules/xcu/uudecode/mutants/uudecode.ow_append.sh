# uudecode appends to an existing file instead of replacing it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref ow_append "$@"; }
pc_uu_install ow_append
