# uudecode turns empty data into one NUL byte
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_empty_nonempty "$@"; }
pc_uu_install r_empty_nonempty
