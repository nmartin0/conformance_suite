# uudecode applies the creation mask to the mode from the data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref mode_umask "$@"; }
pc_uu_install mode_umask
