# uudecode keeps only the owner bits of the mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref mode_owner_only "$@"; }
pc_uu_install mode_owner_only
