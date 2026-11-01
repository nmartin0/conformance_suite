# uudecode adds a newline after the bytes on standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref stdout_nl "$@"; }
pc_uu_install stdout_nl
