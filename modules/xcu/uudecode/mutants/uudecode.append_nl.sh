# uudecode adds a newline to the decoded bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref append_nl "$@"; }
pc_uu_install append_nl
