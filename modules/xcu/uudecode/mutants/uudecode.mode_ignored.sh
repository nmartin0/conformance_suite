# uudecode does not set the mode from the data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref mode_ignored "$@"; }
pc_uu_install mode_ignored
