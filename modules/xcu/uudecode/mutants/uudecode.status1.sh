# uudecode exits 1 after decoding successfully
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref status1 "$@"; }
pc_uu_install status1
