# uudecode drops the last byte when the data is 45 bytes or more
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_ge45_drop "$@"; }
pc_uu_install r_ge45_drop
