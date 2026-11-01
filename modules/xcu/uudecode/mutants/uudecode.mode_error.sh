# uudecode treats a mode that cannot be set as an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref mode_error "$@"; }
pc_uu_install mode_error
