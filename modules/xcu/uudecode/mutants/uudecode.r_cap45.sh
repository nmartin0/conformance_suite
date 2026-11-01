# uudecode stops after 45 bytes (one line)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_cap45 "$@"; }
pc_uu_install r_cap45
